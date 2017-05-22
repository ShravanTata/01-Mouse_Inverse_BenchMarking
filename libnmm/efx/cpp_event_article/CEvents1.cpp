// This is the main project file for VC++ application project 
// generated using an Application Wizard.

//#include "stdafx.h"

//#using <mscorlib.dll>
//#include <tchar.h>
#include <vector>

using namespace System;
using namespace std;

#include "CppEvent.h"

class MySubject
{
public:

	CppEvent1<bool,int> int_event;
	CppEvent2<bool,double,int> double_event;
	CppEvent3<bool,double,int,const char*> triple_event;

	void submit_int()
	{
		int_event.notify(1);
	}

	void submit_double()
	{
		double_event.notify(10.5,100);
	}

	void submit_triple()
	{
		triple_event.notify(10.5,100,"Oh ye");
	}
};

class MyListener1
{
public:

	bool update_int(int p)
	{
		Console::WriteLine(S"int update listener 1");
		return true;
	}

	bool update_double(double p,int p1)
	{
		Console::WriteLine(S"double update listener 1");
		return true;
	}

	bool update_triple(double p,int p1,const char* str)
	{
		Console::WriteLine(S"triple update listener 1");
		return true;
	}
};

class MyListener2
{
public:

	bool fun(int p)
	{
		Console::WriteLine(S"fun update listener 2");
		return true;
	}
};

// This is the entry point for this application
int _tmain(void)
{	 
	// TODO: Please replace the sample code below with your own.
    Console::WriteLine(S"Hello World");
	MyListener1* listener1 = new MyListener1;
	MyListener2* listener2 = new MyListener2;

	MySubject subject;
	
	CppEventHandler h1 = subject.int_event.attach(listener1,&MyListener1::update_int);
	CppEventHandler h2 = subject.int_event.attach(listener2,&MyListener2::fun);
	
	CppEventHandler h3 = subject.double_event.attach(listener1,&MyListener1::update_double);
    CppEventHandler h4 = subject.triple_event.attach(listener1,&MyListener1::update_triple);

	subject.submit_int();
	subject.submit_double();
	subject.submit_triple();

	// detach handlers
	subject.int_event.detach(h1);
	subject.int_event.detach(h2);
	subject.double_event.detach(h3);
	subject.triple_event.detach(h4);

	return 0;
}
