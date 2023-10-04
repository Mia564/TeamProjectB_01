#include "sprite.h"
#include "misc.h"
#include "texture.h"
#include "shader.h"

#include <WICTextureLoader.h>

Sprite::Sprite(ID3D11Device* device, const wchar_t* filename) {
    // ���_���̃Z�b�g
    Vertex vertices[]{
        {{ -1.0, 1.0,0 },{1,1,1,1},{0,0}}, // ����
        {{ 1.0, 1.0,0 },{1,1,1,1},{1,0}},  // �E��
        {{ -1.0, -1.0,0 },{1,1,1,1},{0,1}},// ����
        {{ 1.0, -1.0,0 },{1,1,1,1},{0,1}}, // �E��
    };
    // ���_�o�b�t�@�̃I�u�W�F�N�g�̐���
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(vertices); // �����ɓ���郁�����T�C�Y�̓f�[�^�̐��Ԃ� Vertex * 4�ł�ok
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA subresourceData = {}; // �������f�[�^
    subresourceData.pSysMem = vertices;
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;
    
    HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    // ���_�V�F�[�_�[�I�u�W�F�N�g�̐���
    const char* csoName = "./Shader/sprite_vs.cso";

    // ���̓��C�A�E�g�I�u�W�F�N�g�̐���
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[]{
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
        D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,
        D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
         D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };

    create_vs_from_cso(device, csoName, vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));

    // �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐���
    csoName = "./Shader/sprite_ps.cso";

    create_ps_from_cso(device, csoName, pixelShader.GetAddressOf());

    // �摜�t�@�C���̃��[�h�ƃV�F�[�_�[���\�[�X�r���[�I�u�W�F�N�g(ID3D11ShaderResourceView)�̐���
    // �e�N�X�`�����(D3D11_TEXTURE2D_DESC)�̎擾
    load_texture_from_file(device, filename, shaderResourceView.GetAddressOf(), &texture2dDesc);
    release_all_textures();
}

// sprite�N���X��render�����o�֐��̎���
void Sprite::render(ID3D11DeviceContext* immediateContext,
    float dx, float dy,                     // ��`�̍���̍��W(�X�N���[�����W�n)
    float dw, float dh,                     // ��`�̃T�C�Y(�X�N���[�����W�n)
    float r, float g, float b, float a,     // ��`�̕`��F
    float angle/*degree*/                   // ��]�p
) { 
    render(immediateContext, dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
}

void Sprite::render(ID3D11DeviceContext* immediateContext,
    float dx, float dy, float dw, float dh,
    float r, float g, float b, float a,
    float angle,/*degree*/
    float sx, float sy, float sw, float sh
) {
    // �X�N���[��(�r���[�|�[�g)�̃T�C�Y���擾����
    D3D11_VIEWPORT viewport = {};
    UINT numViewports = 1;
    immediateContext->RSGetViewports(&numViewports, &viewport);

    // left-top
    float x0 = dx;
    float y0 = dy;
    // right-top
    float x1 = dx + dw;
    float y1 = dy;
    // left-bottom
    float x2 = dx;
    float y2 = dy + dh;
    // right-bottom
    float x3 = dx + dw;
    float y3 = dy + dh;

    // UV���W
    float u0 = sx / texture2dDesc.Width;
    float v0 = sy / texture2dDesc.Height;
    float u1 = (sx + sw) / texture2dDesc.Width;
    float v1 = (sy + sh) / texture2dDesc.Height;

    // ��]�̒��S����`�̒��S�_�ɂ����ꍇ
    float cx = dx + dw * 0.5f;
    float cy = dy + dh * 0.5f;
    rotate(x0, y0, cx, cy, angle);
    rotate(x1, y1, cx, cy, angle);
    rotate(x2, y2, cx, cy, angle);
    rotate(x3, y3, cx, cy, angle);

    // �X�N���[�����W�n����NDC�ւ̍��W�ϊ��������Ȃ�
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;

    // �v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
    HRESULT hr = S_OK;
    D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
    hr = immediateContext->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    Vertex* vertices = reinterpret_cast<Vertex*>(mappedSubresource.pData);
    if (vertices != nullptr) {
        vertices[0].position = { x0,y0,0 };
        vertices[1].position = { x1,y1,0 };
        vertices[2].position = { x2,y2,0 };
        vertices[3].position = { x3,y3,0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

        vertices[0].texcoord = { u0,v0 };
        vertices[1].texcoord = { u1,v0 };
        vertices[2].texcoord = { u0,v1 };
        vertices[3].texcoord = { u1,v1 };
    }
    immediateContext->Unmap(vertexBuffer.Get(), 0);

    // ���_�o�b�t�@�[�̃o�C���h
    UINT stride = sizeof(Vertex); // �����͌^�̃T�C�Y������
    UINT offset = 0;
    immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    // �v���~�e�B�u�^�C�v����уf�[�^�̏����Ɋւ�����̃o�C���h
    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    // ���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    immediateContext->IASetInputLayout(inputLayout.Get());
    // �V�F�[�_�[�̃o�C���h
    immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);
    immediateContext->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
    // �v���~�e�B�u�̕`��
    immediateContext->Draw(4, 0);
    
}

// sprite�N���X��render�����o�֐��̎���
void Sprite::render(ID3D11DeviceContext* immediateContext,
    float dx, float dy,                     // ��`�̍���̍��W(�X�N���[�����W�n)
    float dw, float dh                      // ��`�̃T�C�Y(�X�N���[�����W�n)
) {
    render(immediateContext, dx, dy, dw, dh, 1.0f, 1.0f, 1.0f, 1.0f, 0, 0.0f, 0.0f, static_cast<float>(texture2dDesc.Width), static_cast<float>(texture2dDesc.Height));
}

void Sprite::textout(ID3D11DeviceContext* immediateContext, std::string s,
    float x, float y, float w, float h, float r, float g, float b, float a) {

    float sw = static_cast<float>(texture2dDesc.Width / 16);
    float sh = static_cast<float>(texture2dDesc.Height / 16);
    float carriage = 0;
    for (const char c : s) {
        render(immediateContext, x + carriage, y, w, h, r, g, b, a, 0, sw * (c & 0x0F), sh * (c >> 4), sw, sh);
        carriage += w;
    }
}

Sprite::~Sprite() {

}