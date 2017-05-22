# Mouse_Webots_BenchMarking
Common repository for Webots benchmarking of mouse locomotion

## Instructions to install libnmm library

> Execute the following commands from the root of the project

    cd libnmm
    
> Installation spinal dynamics will request for your user account password to install the library in usr/local
    
    ./compile spinalDynamics
    ./compile 5 DISABLE_OPTIMIZATION
    export PKG_CONFIG_PATH=~/.local/usr/lib/pkgconfig:$PKG_CONFIG_PATH
    
> To Check if the above isntallation worked properly

    pkg-config --cflags sml-5.0
    
> The output should be : -I$HOME/.local/usr/include/sml-5.0
> To make a permanent changes add the above export lines to your bashrc or your preferred shell

## Other Instructions

> Compile the ekerbergMouse_wbt8
