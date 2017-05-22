#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>

// Forward declarations
class Spaceship;
typedef boost::shared_ptr<Spaceship> SpaceshipPtr;
typedef boost::weak_ptr<Spaceship> SpaceshipWPtr;

class EventManager;
typedef boost::shared_ptr<EventManager> EventManagerPtr;

class EventManager
{
public:
    // Notify listeners of all recent events
    void TriggerAllQueuedEvents()
    {
        NotificationVec vecNotifications;

        // Open a protected scope to modify the notification list
        {
            // One thread at a time
            boost::recursive_mutex::scoped_lock lock( m_notificationProtection );

            // Copy the notification vector to our local list and clear it at the same time
            std::swap( vecNotifications, m_vecQueuedNotifications );
        }

        // Now loop over the notification callbacks and call each one.
        // Since we're looping over the copy we just made, new events won't affect us.
        BOOST_FOREACH( const EventNotificationFn & fn, vecNotifications )
        {
            fn() ;
        }
    }

    // Callback signature
    typedef void EventNotificationFnSignature();
    typedef boost::function<EventNotificationFnSignature> EventNotificationFn;

    //! Queue an event with the event manager
    void QueueEvent( const EventNotificationFn & event )
    {
        // One thread at a time.
        boost::recursive_mutex::scoped_lock lock(  m_notificationProtection );

        m_vecQueuedNotifications.push_back(event);
    }

private:
    // Queue of events
    typedef std::vector<EventNotificationFn> NotificationVec ;
    NotificationVec m_vecQueuedNotifications;

    // This mutex is used to ensure one-at-a-time access to the list of notifications
    boost::recursive_mutex m_notificationProtection ;
};


class Spaceship
{
public:
    Spaceship(const std::string & name, const EventManagerPtr & pEventManager)
    : m_name(name)
    , m_pEventManager(pEventManager)
    {
    }

    const std::string& name()
    {
        return m_name;
    }

    // Define what a handler for crash events must look like
    typedef void CrashEventHandlerFnSignature(const std::string & sound);
    typedef boost::function<CrashEventHandlerFnSignature> CrashEventHandlerFn;

    // Call this function to be notified of crash events
    boost::signals2::connection subscribeToCrashEvents( const CrashEventHandlerFn & fn )
    {
        return m_crashSignal.connect(fn);
    }

    // Define what a handler for mutiny events must look like
    typedef void MutinyEventHandlerFnSignature(bool mutinyWasSuccessful, int numDeadCrew);
    typedef boost::function<MutinyEventHandlerFnSignature> MutinyEventHandlerFn;

    // Call this function to be notified of mutiny events
    boost::signals2::connection subscribeToMutinyEvents( const MutinyEventHandlerFn & fn )
    {
        return m_mutinySignal.connect(fn);
    }

    // Define what a handler for generic events must look like
    typedef void GenericEventHandlerFnSignature();
    typedef boost::function<GenericEventHandlerFnSignature> GenericEventHandlerFn;

    // Call this function to be notified of generic events
    boost::signals2::connection subscribeToGenericEvents( const std::string & eventType, const GenericEventHandlerFn & fn )
    {
        if ( m_genericEventSignals[eventType] == NULL )
        {
            m_genericEventSignals[eventType].reset( new GenericEventSignal );
        }
        return m_genericEventSignals[eventType]->connect(fn);
    }

    void CauseCrash( const std::string & sound )
    {
        // The ship has crashed.  Queue the event with the event manager.
        m_pEventManager->QueueEvent( boost::bind( boost::ref(m_crashSignal), sound ) ); //< Must use boost::ref because signal is noncopyable.
    }

    void CauseMutiny( bool successful, int numDied )
    {
        // A mutiny has occurred.  Queue the event with the event manager
        m_pEventManager->QueueEvent( boost::bind( boost::ref(m_mutinySignal), successful, numDied ) ); //< Must use boost::ref because signal is noncopyable.
    }

    void CauseGenericEvent( const std::string & eventType )
    {
        // Queue the event with the event manager
        m_pEventManager->QueueEvent( boost::bind( boost::ref(*m_genericEventSignals[eventType]) ) ); //< Must use boost::ref because signal is noncopyable.
    }

private:
    std::string m_name;
    EventManagerPtr m_pEventManager;

    boost::signals2::signal<CrashEventHandlerFnSignature> m_crashSignal;
    boost::signals2::signal<MutinyEventHandlerFnSignature> m_mutinySignal;

    // This map needs to use ptrs, because std::map needs a value type that is copyable
    // (boost signals are noncopyable)
    typedef boost::signals2::signal<GenericEventHandlerFnSignature> GenericEventSignal;
    typedef boost::shared_ptr<GenericEventSignal> GenericEventSignalPtr;
    std::map<std::string, GenericEventSignalPtr > m_genericEventSignals;
};

class Controller
{
public:
    Controller( const std::set<SpaceshipPtr> & ships )
    {
        // For every ship, subscribe to all of the events we're interested in.
        BOOST_FOREACH( const SpaceshipPtr & pSpaceship, ships )
        {
            m_ships.insert( pSpaceship );

            // Bind up a weak_ptr in the handler calls (using a shared_ptr would cause a memory leak)
            SpaceshipWPtr wpSpaceship(pSpaceship);

            // Register event callback functions with the spaceship so he can notify us.
            // Bind a pointer to the particular spaceship so we know who originated the event.
           boost::signals2::connection crashConnection = pSpaceship->subscribeToCrashEvents(
               boost::bind( &Controller::HandleCrashEvent, this, wpSpaceship, _1 ) );

           boost::signals2::connection mutinyConnection = pSpaceship->subscribeToMutinyEvents(
               boost::bind( &Controller::HandleMutinyEvent, this, wpSpaceship, _1, _2 ) );

           // Callbacks for generic events
           boost::signals2::connection takeoffConnection =
               pSpaceship->subscribeToGenericEvents(
                   "takeoff",
                   boost::bind( &Controller::HandleGenericEvent, this, wpSpaceship, "takeoff" ) );

           boost::signals2::connection landingConnection =
               pSpaceship->subscribeToGenericEvents(
                   "landing",
                   boost::bind( &Controller::HandleGenericEvent, this, wpSpaceship, "landing" ) );

           // Cache these connections to make sure we get notified
           m_allConnections[pSpaceship].push_back( crashConnection );
           m_allConnections[pSpaceship].push_back( mutinyConnection );
           m_allConnections[pSpaceship].push_back( takeoffConnection );
           m_allConnections[pSpaceship].push_back( landingConnection );
        }
    }

    ~Controller()
    {
        // Disconnect from any signals we still have
        BOOST_FOREACH( const SpaceshipPtr pShip, m_ships )
        {
            BOOST_FOREACH( boost::signals2::connection & conn, m_allConnections[pShip] )
            {
                conn.disconnect();
            }
        }
    }

private:
    typedef std::vector<boost::signals2::connection> ConnectionVec;
    std::map<SpaceshipPtr, ConnectionVec> m_allConnections;
    std::set<SpaceshipPtr> m_ships;

    void HandleGenericEvent( SpaceshipWPtr wpSpaceship, const std::string & eventType )
    {
        // Obtain a shared ptr from the weak ptr
        SpaceshipPtr pSpaceship = wpSpaceship.lock();

        std::cout << "Event on " << pSpaceship->name() << ": " << eventType << '\n';
    }

    void HandleCrashEvent(SpaceshipWPtr wpSpaceship, const std::string & sound)
    {
        // Obtain a shared ptr from the weak ptr
        SpaceshipPtr pSpaceship = wpSpaceship.lock();

        std::cout << pSpaceship->name() << " crashed with sound: " << sound << '\n';

        // That ship is dead.  Delete it from the list of ships we track.
        m_ships.erase(pSpaceship);

        // Also, make sure we don't get any more events from it
        BOOST_FOREACH( boost::signals2::connection & conn, m_allConnections[pSpaceship] )
        {
            conn.disconnect();
        }
        m_allConnections.erase(pSpaceship);
    }

    void HandleMutinyEvent(SpaceshipWPtr wpSpaceship, bool mutinyWasSuccessful, int numDeadCrew)
    {
        SpaceshipPtr pSpaceship = wpSpaceship.lock();
        std::cout << (mutinyWasSuccessful ? "Successful" : "Unsuccessful" ) ;
        std::cout << " mutiny on " << pSpaceship->name() << "! (" << numDeadCrew << " dead crew members)\n";
    }
};

int main()
{
    // Instantiate an event manager
    EventManagerPtr pEventManager( new EventManager );

    // Create some ships to play with
    int numShips = 5;
    std::vector<SpaceshipPtr> vecShips;
    for (int shipIndex = 0; shipIndex < numShips; ++shipIndex)
    {
        std::string name = "Ship #" + boost::lexical_cast<std::string>(shipIndex);
        SpaceshipPtr pSpaceship( new Spaceship(name, pEventManager) );
        vecShips.push_back(pSpaceship);
    }

    // Create the controller with our ships
    std::set<SpaceshipPtr> setShips( vecShips.begin(), vecShips.end() );
    Controller controller(setShips);

    // Quick-and-dirty "simulation"
    // We'll cause various events to happen to the ships in the simulation,
    // And periodically flush the events by triggering the event manager

    std::cout << "BEGIN Orbit #1" << std::endl;
    vecShips[0]->CauseGenericEvent("takeoff");
    vecShips[0]->CauseCrash("Kaboom!");
    vecShips[1]->CauseGenericEvent("takeoff");
    vecShips[1]->CauseCrash("Blam!");
    vecShips[2]->CauseGenericEvent("takeoff");
    vecShips[2]->CauseMutiny(false, 7);
    std::cout << "END Orbit #1" << std::endl;

    pEventManager->TriggerAllQueuedEvents();

    std::cout << "BEGIN Orbit #2" << std::endl;
    vecShips[3]->CauseGenericEvent("takeoff");
    vecShips[3]->CauseMutiny(true, 2);
    vecShips[3]->CauseGenericEvent("takeoff");
    vecShips[4]->CauseCrash("Splat!");
    std::cout << "END Orbit #2" << std::endl;

    pEventManager->TriggerAllQueuedEvents();

    std::cout << "BEGIN Orbit #3" << std::endl;
    vecShips[2]->CauseMutiny(false, 15);
    vecShips[2]->CauseMutiny(true, 20);
    vecShips[2]->CauseGenericEvent("landing");
    vecShips[3]->CauseCrash("Fizzle");
    vecShips[3]->CauseMutiny(true, 0); //< Should not cause output, since this ship has already crashed!
    std::cout << "END Orbit #3" << std::endl;

    pEventManager->TriggerAllQueuedEvents();

    return 0;
}
