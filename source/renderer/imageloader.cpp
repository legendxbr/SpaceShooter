#include "imageloader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <window/device.hpp>

bool images::loadfile( const char* filename, ID3D11ShaderResourceView** out_srv, float* out_width, float* out_height )
{
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load( filename, &image_width, &image_height, NULL, 4 );
    if ( image_data == NULL )
        return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof( desc ) );
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    
    ID3D11Texture2D *pTexture = NULL;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;
    device::vars::g_pd3dDevice->CreateTexture2D( &desc, &subResource, &pTexture );
    
    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory( &srvDesc, sizeof( srvDesc ) );
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    device::vars::g_pd3dDevice->CreateShaderResourceView( pTexture, &srvDesc, out_srv );
    pTexture->Release( );

    *out_width = image_width;
    *out_height = image_height;
    stbi_image_free( image_data );

    return true;
}

void images::renderfromsheet( ID3D11ShaderResourceView* texture, ImVec2 min, ImVec2 max, int index, int indexy, ImVec2 spriteSize, ImVec2 sheetSize, ImColor color, ImDrawList* list )
{
    ImVec2 uv0 = ImVec2( (0 + ( 80 * index )) / sheetSize.x, ( 0 + ( 80 * indexy ) ) / sheetSize.y );
    ImVec2 uv1 = ImVec2( ( 0 + ( 80 * index ) + spriteSize.x ) / sheetSize.x, ( 0 + ( 80 * indexy ) + spriteSize.y ) / sheetSize.y );

    if ( list == nullptr ) {
        ImGui::GetBackgroundDrawList( )->AddImage( texture, min, max, uv0, uv1, color );
    }
    else {
        list->AddImage( texture, min, max, uv0, uv1, color );
    }
}

void images::rotated( ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle, int index, int indexy, ImVec2 spriteSize, ImVec2 sheetSize, ImColor color )
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList( );

    float cos_a = cosf( angle );
    float sin_a = sinf( angle );
    ImVec2 pos[ 4 ] =
    {
        center + ImRotate( ImVec2( -size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( +size.x * 0.5f, -size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( +size.x * 0.5f, +size.y * 0.5f ), cos_a, sin_a ),
        center + ImRotate( ImVec2( -size.x * 0.5f, +size.y * 0.5f ), cos_a, sin_a )
    };

    ImVec2 uv0 = ImVec2( ( 0 + ( 80 * index ) ) / sheetSize.x, ( 0 + ( 80 * indexy ) ) / sheetSize.y );
    ImVec2 uv1 = ImVec2( ( 0 + ( 80 * index ) + spriteSize.x ) / sheetSize.x, ( 0 + ( 80 * indexy ) + spriteSize.y ) / sheetSize.y );

    ImVec2 uvs[ 4 ] =
    {
        ImVec2( uv0.x, uv0.y ),
        ImVec2( uv1.x, uv0.y ),
        ImVec2( uv1.x, uv1.y ),
        ImVec2( uv0.x, uv1.y )
    };

    draw_list->AddImageQuad( tex_id, pos[ 0 ], pos[ 1 ], pos[ 2 ], pos[ 3 ], uvs[ 0 ], uvs[ 1 ], uvs[ 2 ], uvs[ 3 ], color );
}
