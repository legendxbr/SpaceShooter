#include <common.hpp>
#include <window/device.hpp>
#include <window/window.hpp>
#include <renderer/renderer.hpp>

int main(int, char**)
{
    printf( "[+] creating window\n" );
    if ( !window::create( "imgui", 600, 600 ) ) {
        MessageBoxA( 0, "couldn't create window", 0, 0 );
        return false;
    }
  
    printf( "[+] starting renderer\n" );
    if ( !renderer::start( ) ) {
        MessageBoxA( 0, "couldn't start renderer", 0, 0 );
        return false;
    }

    printf( "[+] rendering\n" );
    renderer::update( true );
    renderer::shutdown( );
    
    return 0;
}
