## Settings and Parameter Manager for C++

### Introduction
Simple class that can be used to manage parameters associated with a class that we would like to load from `file` , `stream` or directly from the code.

A string is associated to each parameter.

### Example
> All __USAGE__ case can be found in the test suits.
Simple example case are shown here for convience


- Parameter creation with list constructor
```cpp
Parameters param = Parameters::Create<double>("p",1.0,"d",2.0,"k",3.0,"x0",4.0);

REQUIRE( param.get<double>("p") == 1.0);
REQUIRE( param.get<double>("d") == 2.0);
REQUIRE( param.get<double>("k") == 3.0); 

REQUIRE( param.get<std::string>("p") == "");
REQUIRE( param.get<std::string>("d") == "");
REQUIRE( param.get<std::string>("k") == "");
```

- Parameter creation from stream
```cpp
std::stringstream config;
config << "- parameters : double" << std::endl;
config << "  p1 : 1.0" << std::endl;
config << "  p2 : 2.0" << std::endl;
config << "  p3 : 3.0" << std::endl;

Parameters param = Parameters::CreateFromStream<std::stringstream>(config);

REQUIRE( param.get<double>("p1") == 1.0);
REQUIRE( param.get<double>("p2") == 2.0);
REQUIRE( param.get<int>("p2") == 0);
```

- Creation from file

Given a file `parameters_test.txt` with the following content

```yaml
- parameters : double
  p1 : 0.1
  p2 : 0.2
  p3 : 0.3
- parameters : bool
  b1 : true
  b2 : true
  b3 : false
- parameters : string
  list_name  :  muscles
  list_content : A B C D E F
- parameters : double
  list_name  :  muscles_length
  list_content : 0.1 0.1 0.1
- parameters : table
  headers_type : string double double bool
  headers_name : muscles_names muscles_pennation muscles_forces muscles_locked
  content : A 0.1 100 true
            B 0.2 200 true
            C 0.3 300 true
```

The parameter object can be populated with 
```cpp
Parameters param2 = Parameters::CreateFromFile("parameters_test.txt");
       
```
or 
```cpp
Parameters param2;
Parameters::LoadFromFile("parameters_test.txt");
```

After creation the object `param2` contains all the parameter present in the file
```cpp

REQUIRE( param2.get<double>("p1") == 0.1);
REQUIRE( param2.get<double>("p2") == 0.2);
REQUIRE( param2.get<double>("p3") == 0.3);

REQUIRE( param2.get<bool>("b1") == true);
REQUIRE( param2.get<bool>("b2") == true);
REQUIRE( param2.get<bool>("b3") == false);

REQUIRE( param2.getVector<double>("muscles_length")[0] == 0.1);
REQUIRE( param2.getVector<std::string>("muscles")[0] == "A");
REQUIRE( param2.getVector<std::string>("muscles")[1] == "B");
REQUIRE( param2.getVector<std::string>("muscles")[2] == "C");

REQUIRE( param2.getVector<std::string>("muscles_names")[0] == "A");
REQUIRE( param2.getVector<std::string>("muscles_names")[1] == "B");
REQUIRE( param2.getVector<std::string>("muscles_names")[2] == "C");

REQUIRE( param2.getVector<double>("muscles_pennation")[0] == 0.1);
REQUIRE( param2.getVector<double>("muscles_pennation")[1] == 0.2);
REQUIRE( param2.getVector<double>("muscles_pennation")[2] == 0.3);

REQUIRE( param2.getVector<double>("muscles_forces")[0] == 100);
REQUIRE( param2.getVector<double>("muscles_forces")[1] == 200);
REQUIRE( param2.getVector<double>("muscles_forces")[2] == 300);

REQUIRE( param2.getVector<bool>("muscles_locked")[0] == true);
REQUIRE( param2.getVector<bool>("muscles_locked")[1] == true);
REQUIRE( param2.getVector<bool>("muscles_locked")[2] == true);

```


