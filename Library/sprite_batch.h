#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <vector>


class SpriteBatch {
private:
    // �����o
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
    D3D11_TEXTURE2D_DESC texture2dDesc;
public:
    // �����o�֐�
    void render(ID3D11DeviceContext* immediateContext,
        float dx, float dy, float dw, float dh,
        float r, float g, float b, float a,
        float angle
    );

    void render(ID3D11DeviceContext* immediateContext,
        float dx, float dy, float dw, float dh,
        float r, float g, float b, float a,
        float angle,/*degree*/
        float sx, float sy, float sw, float sh
    );

    void render(ID3D11DeviceContext* immediateContext,
        float dx, float dy, float dw, float dh
    );

    void begin(ID3D11DeviceContext* immediateContext);
    void end(ID3D11DeviceContext* immediateContext);

    // �_(x,y)���_(cx,cy)�𒆐S�Ɋp(angle)�ŉ�]�������̍��W���v�Z����֐�(inline)
    void rotate(float& x, float& y, float cx, float cy, float angle) {
        x -= cx;
        y -= cy;

        float cos = cosf(DirectX::XMConvertToRadians(angle));
        float sin = sinf(DirectX::XMConvertToRadians(angle));
        float tx = x, ty = y;
        x = cos * tx + -sin * ty;
        y = sin * tx + cos * ty;

        x += cx;
        y += cy;
    }
    // �R���X�g���N�^�E�f�X�g���N�^
    SpriteBatch(ID3D11Device* device, const wchar_t* filename, size_t maxSprites);
    ~SpriteBatch();
    // ���_�t�H�[�}�b�g
    struct Vertex {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 texcoord;
    };
private:
    // �����o�ϐ�
    std::vector<Vertex> vertices;
    const size_t maxVertices;
};
