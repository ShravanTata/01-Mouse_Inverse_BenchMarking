#include <model.hh>

OpenSim::Body& 
ModelExt::getLink(Model& model, std::string name){
    OpenSim::Body& b = const_cast<OpenSim::BodySet&>(model.getBodySet()).get(name);
    //return model.getBodySet().get(name);
    return b;
}
const PinJoint& 
ModelExt::getJoint(Model& model, std::string name){
    return (PinJoint&)(model.getJointSet().get(name));
}
OpenSim::Muscle& 
ModelExt::getMuscle(Model& model, std::string name){

    OpenSim::Force& m = const_cast<OpenSim::ForceSet&>(model.getForceSet()).get(name);
    return (OpenSim::Muscle&)m;
}

void
ModelExt::saveResults(Manager& manager, std::string filename){
    Storage statesRadian(manager.getStateStorage());
    statesRadian.print(filename);
}
void
ModelExt::saveModel(Model& model, std::string filename){
    model.print(filename);
}


void 
ModelBase::updBackground( Visualizer::BackgroundType bg = Visualizer::BackgroundType::SolidColor){
    model.updVisualizer().updSimbodyVisualizer().setBackgroundType(bg);
};

ModelBase::ModelBase(){

    /**
     * Visualization parameters
     */
    parameters.get<bool>("visualize") = false;
    parameters.get<Visualizer::BackgroundType>("visualize/background_type") = Visualizer::BackgroundType::SolidColor;
    /**
     * Model parameters
     */
    parameters.get<bool>("save_model") = false;
    parameters.get<std::string>("save_model/filename") = "default.osim";
    parameters.get<bool>("load_from_file") = false;
    parameters.get<std::string>("load_from_file/filename") = "default.osim";
    /**
     * Simulation parameters
     */
    parameters.get<bool>("save_simulation_results") = false;
    parameters.get<std::string>("save_simulation_results/filename") = "results.sto";



};
void ModelBase::simulate(){
    double duration = parameters.get<double>("duration");       

    

    simulate_pre();

    //RungeKuttaMersonIntegrator integrator(model.getSystem());
    //RungeKutta2Integrator integrator(model.getSystem());
    ExplicitEulerIntegrator integrator(model.getSystem());
    //SemiExplicitEuler2Integrator integrator(model.getSystem());
    //VerletIntegrator integrator(model.getSystem(),1.0e-3);
    integrator.setFixedStepSize(1.0e-3);
    Manager manager(model, integrator);
    manager.setInitialTime(0); manager.setFinalTime(duration);
    manager.integrate(getState(),1.0e-3);

    if ( parameters.get<bool>("save_simulation_results")){
        auto& filename = parameters.get<std::string>("save_simulation_results/filename");
        std::cout << "Saving results to file : " << filename << std::endl;
        ModelExt::saveResults(manager,filename);
    }

    simulate_post();


};

void 
ModelBase::init(){
    /*** Parameters loading ***/
    if ( parameters.get<bool>("load_parameters")){
    parameters.loadFromFile(parameters.get<std::string>("load_parameters/filename"));
    }
    init_parameters();




    /*** Model creation ***/
    if ( parameters.get<bool>("load_from_file")){
        auto& filename = parameters.get<std::string>("load_from_file/filename");
        model = Model(filename);
    }

    if ( parameters.get<bool>("visualize"))
        model.setUseVisualizer(true);

    /*** Body details ***/
    if ( !parameters.get<bool>("load_from_file")){
        init_body();
    }





    /*** Body contacts ***/
    if ( parameters.get<bool>("contact")){
        OpenSim::Body& ground = model.getGroundBody(); 
        // Define contact geometry
        // Create new floor contact halfspace


        // Contact parameters
        double stiffness = 1.0e7, dissipation = 0.3, friction = 0.2, viscosity=0.01;
            // Define contact parameters for elastic foundation force
        OpenSim::ElasticFoundationForce::ContactParameters *contactParams = 
            new OpenSim::ElasticFoundationForce::ContactParameters(stiffness, dissipation, friction, friction, viscosity);
        // Create a new elastic foundation (contact) force between the floor and cube.
        OpenSim::ElasticFoundationForce *contactForce = new OpenSim::ElasticFoundationForce(contactParams);


        // create contact
        ContactHalfSpace *floor = new ContactHalfSpace(SimTK::Vec3(0), SimTK::Vec3(0, 0, -0.5*SimTK_PI), ground, "floor");
        // add contact geometry to the model
        model.addContactGeometry(floor);
        // add geometry to the contact
        contactParams->addGeometry("floor");


        std::vector<std::string> geom_name = std::vector<std::string>(parameters.getVector<std::string>("contact/geom_name"));
        std::vector<std::string> geom_mesh_file = std::vector<std::string>(parameters.getVector<std::string>("contact/mesh_file"));
        std::vector<std::string> geom_body_name = std::vector<std::string>(parameters.getVector<std::string>("contact/body_name"));

        

        for (int i=0; i<geom_name.size(); i++){
            OpenSim::Body* tmpBody = &ModelExt::getLink(model,geom_body_name[i]);
            OpenSim::ContactMesh *tmpMesh = new OpenSim::ContactMesh(geom_mesh_file[i], SimTK::Vec3(0), SimTK::Vec3(0), *tmpBody, geom_name[i]);
            //tmpMesh->setDisplayPreference(3);
            model.addContactGeometry(tmpMesh);

            contactParams->addGeometry(geom_name[i]);

        }

        contactForce->setName("contactForce");

        // Add the new elastic foundation force to the model
        model.addForce(contactForce);
    }




    /*** Controllers initialization ***/

    if ( parameters.get<bool>("controller")){
        std::string controller_type = parameters.get<std::string>("controller/type");
        std::vector<std::string> muscles_name = std::vector<std::string>(parameters.getVector<std::string>("controller/muscles_name"));
        if (controller_type == "TwitchingController"){
            controller = new TwitchingController();
            for( auto &kv : muscles_name){
                controller->addActuator(ModelExt::getMuscle(model,kv));
            }
            static_cast< TwitchingController *>(controller)->init_file();        
            if ( parameters.get<bool>("controller/hasDuration")){

               double duration = static_cast< TwitchingController*>(controller)->get_duration();
               parameters.get<double>("duration") = duration;
            }
            model.addController(controller);

        }
        else if(controller_type == "PrescribedController"){
            controller = new PrescribedController();
            std::vector<double> p0 = std::vector<double>(parameters.getVector<double>("controller/p0"));
            std::vector<double> p1 = std::vector<double>(parameters.getVector<double>("controller/p1"));
            std::vector<double> p2 = std::vector<double>(parameters.getVector<double>("controller/p2"));
            std::vector<double> p3 = std::vector<double>(parameters.getVector<double>("controller/p3"));
            for (int i=0; i<muscles_name.size(); i++){
                controller->addActuator(ModelExt::getMuscle(model,muscles_name[i]));
                static_cast< PrescribedController *>(controller)->prescribeControlForActuator(muscles_name[i],
                    new StepFunction(p0[i],p1[i],p2[i],p3[i]));
            }

            if ( parameters.get<double>("controller/hasDuration")){

               double duration = static_cast< TwitchingController*>(controller)->get_duration();
               parameters.get<double>("duration") = duration;
            }
            model.addController(controller);
        }

        
    }

    init_controller();





    /*** State creation ***/
    state = &model.initSystem();

    if ( parameters.get<bool>("save_model")){
        auto& filename = parameters.get<std::string>("save_model/filename");
        std::cout << "Saving model to file : " << filename << std::endl;
        ModelExt::saveModel(model,filename);
    }



    /*** Body coordinates configuration ***/
    if ( parameters.get<bool>("coordinate")){
        std::vector<std::string> coor_name = std::vector<std::string>(parameters.getVector<std::string>("coordinate/name"));
        std::vector<int> coor_id = std::vector<int>(parameters.getVector<int>("coordinate/id"));
        std::vector<bool> coor_locked = std::vector<bool>(parameters.getVector<bool>("coordinate/locked"));

        // Coordinate id given as parameters
        if (coor_id.size() != 0){
            for (int i=0; i<coor_id.size(); i++){
                cout << "Coordinate id \"" << coor_id[i] << "\"" << " locked state : " << coor_locked[i] << endl;
                model.updCoordinateSet()[i].setLocked(getState(), coor_locked[i]);
            }
        }
        else{
            for (int i=0; i<coor_name.size(); i++){
                cout << "Coordinate \"" << coor_name[i] << "\"" << " locked state : " << coor_locked[i] << endl;
                model.updCoordinateSet().get(coor_name[i]).setLocked(getState(), coor_locked[i]);

            }
        }
    }


    //model.equilibrateMuscles(getState());
    init_post_InitSystem();

    if ( parameters.get<bool>("visualize") ){
        init_visualization();
        updBackground(parameters.get<Visualizer::BackgroundType>("visualize/background_type")); 
        model.updMatterSubsystem().setShowDefaultGeometry(parameters.get<bool>("visualize/geometry"));
    }
};

void 
Arm1Dof2::init_post_InitSystem(){
    // Fix shoulder joint, flex elbow joint.
    model.updCoordinateSet()[1].setValue(getState(), 0.5 * Pi);
    model.equilibrateMuscles(getState());
}


void 
Arm1Dof2::init_body(){
    // Two links, with mass of 1 kg, center of mass at the
    // origin of the body's frame, and moments/products of inertia of zero.
    OpenSim::Body* link1 = new OpenSim::Body("humerus", 1, Vec3(0), Inertia(0));
    OpenSim::Body* link2 = new OpenSim::Body("radius", 1, Vec3(0), Inertia(0));

    // Joints that connect the bodies together.
    PinJoint* joint1 = new PinJoint("shoulder",
            // Parent body, location in parent, orientation in parent.
            model.getGroundBody(), Vec3(0,2.0,0), Vec3(0),
            // Child body, location in child, orientation in child.
            *link1, Vec3(0, 1, 0), Vec3(0));
    PinJoint* joint2 = new PinJoint("elbow",
            *link1, Vec3(0), Vec3(0), *link2, Vec3(0, 1, 0), Vec3(0));

    // Add an actuator that crosses the elbow, and a joint stop.
    Millard2012EquilibriumMuscle* muscle = new
        Millard2012EquilibriumMuscle("biceps", 200, 0.6, 0.55, 0);
    muscle->addNewPathPoint("point1", *link1, Vec3(0, 0.8, 0));
    muscle->addNewPathPoint("point2", *link2, Vec3(0, 0.7, 0));

    // Add bodies and joints to the model.
    model.addBody(link1); model.addBody(link2);
    model.addJoint(joint1); model.addJoint(joint2);
    model.addForce(muscle);
}
void 
Arm1Dof2::init_controller(){

    // // A controller that specifies the excitation of the biceps muscle.
    // PrescribedController* brain = new PrescribedController();
    // brain->addActuator(ModelExt::getMuscle(model,"biceps"));
    // // Muscle excitation is 0.3 for the first 0.5 seconds, and 1.0 thereafter.
    // brain->prescribeControlForActuator("biceps",
    //         new StepFunction(0.5, 3, 0.3, 1));

    // model.addController(brain);
}


void 
Arm1Dof2::init_visualization(){
    if ( !model.getUseVisualizer() )
        return;

    Visualizer& viz = model.updVisualizer().updSimbodyVisualizer();
    
    // Ellipsoids: 0.5 m radius along y axis, centered 0.5 m up along y axis.
    DecorativeEllipsoid geom(Vec3(0.1, 0.5, 0.1)); Vec3 center(0, 0.5, 0);
    viz.addDecoration(ModelExt::getLink(model,"humerus").getMobilizedBodyIndex(), Transform(center), geom);
    viz.addDecoration(ModelExt::getLink(model,"radius").getMobilizedBodyIndex(), Transform(center), geom);
}



void
Cube::init_parameters(){
    parameters.get<Real>("blockSideLength") = 0.1;
    parameters.get<Real>("blockMass") = 20.0;
}

void 
Cube::init_body(){
    Real& blockSideLength = parameters.get<Real>("blockSideLength");
    Real& blockMass = parameters.get<Real>("blockMass");


     // Get a reference to the model's ground body
    OpenSim::Body& ground = model.getGroundBody(); 
      
    // Add display geometry to the ground to visualize in the GUI
    //ground.addDisplayGeometry("ground.vtp");

    // Specify properties of a 20 kg, 0.1 m^3 block body
    Vec3 blockMassCenter(0,0,0);
    Inertia blockInertia = blockMass*Inertia::brick(blockSideLength, blockSideLength, blockSideLength);
       
    // Create a new block body with specified properties
    OpenSim::Body *block = new OpenSim::Body("block", blockMass, blockMassCenter, blockInertia); 
      
    // Add display geometry to the block to visualize in the GUI
    //block->addDisplayGeometry("block.vtp");
    block->addDisplayGeometry(parameters.get<string>("blockGeom"));
    //block->updDisplayer()->setScaleFactors(Vec3(0.2,0.2,0.2));

    // Create a new free joint with 6 degrees-of-freedom (coordinates) between the block and ground bodies
    Vec3 locationInParent(0, blockSideLength*2, 0), orientationInParent(0), locationInBody(0), orientationInBody(0);
    FreeJoint *blockToGround = new FreeJoint("blockToGround", ground, locationInParent, orientationInParent, *block, locationInBody, orientationInBody); 
      
    model.addBody(block);
    model.addJoint(blockToGround);


    // Define the acceleration of gravity
    model.setGravity(Vec3(0,-9.80665,0));

}

void
Cube::init_post_InitSystem(){
    Real& blockSideLength = parameters.get<Real>("blockSideLength");
    Real& blockMass = parameters.get<Real>("blockMass");
    // Define non-zero (defaults are 0) states for the free joint
    CoordinateSet& modelCoordinateSet = model.updCoordinateSet();
      
    // set x-translation value
    modelCoordinateSet[3].setValue(getState(), blockSideLength);
      
    // set x-speed value
    modelCoordinateSet[3].setSpeedValue(getState(), 0.1);
      
    // set y-translation value
    modelCoordinateSet[4].setValue(getState(), blockSideLength/2+0.01);

}


void 
Cube::init_controller(){

}
void 
Cube::init_visualization(){

}
