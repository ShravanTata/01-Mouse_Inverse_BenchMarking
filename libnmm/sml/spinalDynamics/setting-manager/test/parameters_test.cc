#include "catch.hpp"
#include <iostream>
#include <sstream>
#include <parameters.hh>

TEST_CASE( "Parameters", "[parameters]" ) {
    SECTION( "Separated maps for parameters"){
        Parameters param1;
        Parameters param2;

        param1.get<double>("a") = 1.0;
        param2.get<double>("a") = 2.0;

        REQUIRE ( param1.get<double>("a") == 1.0 );
        REQUIRE ( param2.get<double>("a") == 2.0 );

    }
    SECTION( "Pointer parameter handler"){
        double a = 1.0;

        Parameters param;

        param.get<double*>("a") = &a;

        REQUIRE( *param.get<double*>("a") == 1.0);
        a++;
        REQUIRE( *param.get<double*>("a") == 2.0);

    }
    SECTION( "Undefined parameter handler"){
        Parameters param;

        REQUIRE( param.get<std::string>("a") == "");
        REQUIRE( param.get<double>("a") == 0.0);
        REQUIRE( param.getVector<double>("a").size() == 0);
        REQUIRE( param.get<bool>("a") == false);
    }
    SECTION( "Adder / Setter "){
        Parameters param1;

        // default parameter
        REQUIRE( param1.get<double>("a") == 0.0);
        REQUIRE( param1.get<std::string>("a") == "");

        // setting
        param1.get<double>("a") = 1.0;
        REQUIRE( param1.get<double>("a") == 1.0);

        // setting
        param1.get<double>("a") = 2.0;
        REQUIRE( param1.get<double>("a") == 2.0);

        // several parameter with same name and different type

        param1.get<int>("a") = 2;
        REQUIRE( param1.get<int>("a") == 2);
        param1.get<std::string>("a") = "2";
        REQUIRE( param1.get<std::string>("a") != "2.0");
        REQUIRE( param1.get<std::string>("a") == "2");
    }
    SECTION( "Multiple arguments at the same time"){


        Parameters param = Parameters::Create<double>("p",1.0,"d",2.0,"k",3.0,"x0",4.0);

        REQUIRE( param.get<double>("p") == 1.0);
        REQUIRE( param.get<double>("d") == 2.0);
        REQUIRE( param.get<double>("k") == 3.0); 

        REQUIRE( param.get<std::string>("p") == "");
        REQUIRE( param.get<std::string>("d") == "");
        REQUIRE( param.get<std::string>("k") == ""); 
        // param.addList<std::string>("p","1.0","d","2.0","k","3.0","x0","4.0");
        // REQUIRE( param.get<std::string>("p") == "1.0");
        // REQUIRE( param.get<std::string>("d") == "2.0");
        // REQUIRE( param.get<std::string>("k") == "3.0"); 


        param.addList<int>("p",0,"d",1,"k",4,"x0",5);

        REQUIRE( param.get<int>("p") == 0);
        REQUIRE( param.get<int>("d") == 1);
        REQUIRE( param.get<int>("k") == 4); 
    }
    SECTION( "Stream constructor"){
        std::stringstream config;
        config << "- parameters : double" << std::endl;
        config << "  p1 : 1.0" << std::endl;
        config << "  p2 : 2.0" << std::endl;
        config << "  p3 : 3.0" << std::endl;

        Parameters param = Parameters::CreateFromStream<std::stringstream>(config);

        REQUIRE( param.get<double>("p1") == 1.0);
        REQUIRE( param.get<double>("p2") == 2.0);
        REQUIRE( param.get<int>("p2") == 0);
            // ConstParameters pid_param = ConstParameters(config_stream);
        
    }
    SECTION( "File constructor"){
        std::ifstream f("parameters_test.txt");
        std::stringstream s;
        if (f) {
            s << f.rdbuf();    
            f.close();
            //Parameters param2 = Parameters::CreateFromStream<std::stringstream>(s);
            Parameters param2 = Parameters::CreateFromFile("parameters_test.txt");
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

        }
        else{
/* CONTENT OF THE FILE IN CASE IT DOES NOT EXISTS
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
*/
            std::cout << "parameters_test.txt not found" << std::endl;
        }
    }
    SECTION( "Lists in parameters"){
        std::stringstream config;
        config << "- parameters : string" << std::endl;
        config << "  list_name : muscles" << std::endl;
        config << "  list_content : " << std::endl;
        config << "                 B" << std::endl;
        config << "                 C" << std::endl;
        config << "                 D" << std::endl;
        config << "- parameters : double" << std::endl;
        config << "  list_name : muscles_length" << std::endl;
        config << "  list_content : " << std::endl;
        config << "                 0.1" << std::endl;
        config << "                 0.2" << std::endl;
        config << "                 0.3" << std::endl;

        Parameters param = Parameters::CreateFromStream<std::stringstream>(config);

        REQUIRE( param.getVector<std::string>("muscles")[0] == "B");
        REQUIRE( param.getVector<std::string>("muscles")[1] == "C");
        REQUIRE( param.getVector<std::string>("muscles")[2] == "D");

        REQUIRE( param.get<std::vector<std::string>>("muscles")[0] == "B");
        REQUIRE( param.get<std::vector<std::string>>("muscles")[1] == "C");
        REQUIRE( param.get<std::vector<std::string>>("muscles")[2] == "D");

        REQUIRE( param.getVector<double>("muscles_length")[0] == 0.1);
        REQUIRE( param.getVector<double>("muscles_length")[1] == 0.2);
        REQUIRE( param.getVector<double>("muscles_length")[2] == 0.3);


            
    }
}
