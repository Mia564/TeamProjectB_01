#include "geometric_primitive.h"
#include "shader.h"
#include "misc.h"

GeometricPrimitive::GeometricPrimitive(ID3D11Device* device) {
    Vertex vertices[24] = {};
    // �T�C�Y��1.0�̐������̃f�[�^���쐬����(�d�S�����_�Ƃ���)�B�������̂̃R���g���[���|�C���g���� 8 �A
    // 1 �̃R���g���[���|�C���g�̈ʒu�ɂ͖@���̌������Ⴄ���_�� 3 ����̂Œ��_���̑����� 8x3=24 �A
    // ���_���z��ivertices�j�ɂ��ׂĒ��_�̈ʒu�E�@�������i�[����B

    // ��O��
    vertices[0].position = { 0,1,0 };       // ����
    vertices[1].position = { 1,1,0 };       // �E��
    vertices[2].position = { 1,0,0 };       // �E��
    vertices[3].position = { 0,0,0 };       // ����

    vertices[0].normal = { 0,0,-1 };        // ����
    vertices[1].normal = { 0,0,-1 };        // �E��
    vertices[2].normal = { 0,0,-1 };        // �E��
    vertices[3].normal = { 0,0,-1 };        // ����

    // ����
    vertices[4].position = { 0,1,1 };       // ����
    vertices[5].position = { 1,1,1 };       // �E��
    vertices[6].position = { 1,0,1 };       // �E��
    vertices[7].position = { 0,0,1 };       // ����

    vertices[4].normal = { 0,0,1 };         // ����
    vertices[5].normal = { 0,0,1 };         // �E��
    vertices[6].normal = { 0,0,1 };         // �E��
    vertices[7].normal = { 0,0,1 };         // ����

    // ���
    vertices[8].position = { 1,1,1 };       // ����
    vertices[9].position = { 0,1,1 };       // �E��
    vertices[10].position = { 1,1,0 };      // �E��
    vertices[11].position = { 0,1,0 };      // ����

    vertices[8].normal = { 0,1,0 };         // ����
    vertices[9].normal = { 0,1,0 };         // �E��
    vertices[10].normal = { 0,1,0 };        // �E��
    vertices[11].normal = { 0,1,0 };        // ����

    // ����
    vertices[12].position = { 0,0,0 };      // ����
    vertices[13].position = { 1,0,0 };      // �E��
    vertices[14].position = { 0,0,1 };      // �E��
    vertices[15].position = { 1,0,1 };      // ����

    vertices[12].normal = { 0,-1,0 };       // ����
    vertices[13].normal = { 0,-1,0 };       // �E��
    vertices[14].normal = { 0,-1,0 };       // �E��
    vertices[15].normal = { 0,-1,0 };       // ����

    // ����
    vertices[16].position = { 0,1,1 };      // ����
    vertices[17].position = { 0,1,0 };      // �E��
    vertices[18].position = { 0,0,0 };      // �E��
    vertices[19].position = { 0,0,1 };      // ����

    vertices[16].normal = { -1,0,0 };       // ����
    vertices[17].normal = { -1,0,0 };       // �E��
    vertices[18].normal = { -1,0,0 };       // �E��
    vertices[19].normal = { -1,0,0 };       // ����

    // �E��
    vertices[20].position = { 1,1,0 };      // ����
    vertices[21].position = { 1,1,1 };      // �E��
    vertices[22].position = { 1,0,1 };      // �E��
    vertices[23].position = { 1,0,0 };      // ����

    vertices[20].normal = { 1,0,0 };        // ����
    vertices[21].normal = { 1,0,0 };        // �E��
    vertices[22].normal = { 1,0,0 };        // �E��
    vertices[23].normal = { 1,0,0 };        // ����

    uint32_t indices[36] = {};
    // �������̂�6�ʎ����A1�̖ʂ�2��3�p�`�|���S���ō\�������̂�3�p�`�|���S���̑�����6�~2��12�A
    // �������̂�`�悷�邽�߂�12���3�p�|���S���`�悪�K�v�A����ĎQ�Ƃ���钸�_����12�~3��36��A
    // 3�p�`�|���S�����Q�Ƃ��钸�_���̃C���f�b�N�X(���_�ԍ�)��`�揇�ɔz��(indices)�Ɋi�[����B
    // ���v��肪�\�ʂɂȂ�悤�Ɋi�[���邱�ƁB

    // ��O�� 0~3
    indices[0] = 0; indices[1] = 1; indices[2] = 2; 
    indices[3] = 0; indices[4] = 2; indices[5] = 3;

    // ���� 4~7
    indices[6] = 4; indices[7] = 6; indices[8] = 5;
    indices[9] = 4; indices[10] = 7; indices[11] = 6;

    // ��� 8~11
    indices[12] = 11; indices[13] = 9; indices[14] = 8;
    indices[15] = 8; indices[16] = 10; indices[17] = 11;

    // ���� 12~15
    indices[18] = 12; indices[19] = 13; indices[20] = 14;
    indices[21] = 13; indices[22] = 15; indices[23] = 14;

    // ���� 16~19
    indices[24] = 16; indices[25] = 17; indices[26] = 18;
    indices[27] = 16; indices[28] = 18; indices[29] = 19;

    // ���� 20~23
    indices[30] = 20; indices[31] = 21; indices[32] = 22;
    indices[33] = 20; indices[34] = 22; indices[35] = 23;

    create_com_buffers(device, vertices, 24, indices, 36);

    HRESULT hr = S_OK;

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
        D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };
    
    create_vs_from_cso(device, "geometric_primitive_vs.cso", vertexShader.GetAddressOf(),
        inputLayout.GetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
    create_ps_from_cso(device, "geometric_primitive_ps.cso", pixelShader.GetAddressOf());

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Constants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void GeometricPrimitive::create_com_buffers(ID3D11Device* device, Vertex* vertices, size_t vertexCount,
    uint32_t* indices, size_t indexCount) {
    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC bufferDesc = {};
    D3D11_SUBRESOURCE_DATA subresourceData = {};
    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertexCount);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    subresourceData.pSysMem = vertices;
    subresourceData.SysMemPitch = 0;
    subresourceData.SysMemSlicePitch = 0;
    hr = device->CreateBuffer(&bufferDesc, &subresourceData, vertexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indexCount);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresourceData.pSysMem = indices;
    hr = device->CreateBuffer(&bufferDesc, &subresourceData, indexBuffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void GeometricPrimitive::render(ID3D11DeviceContext* immediateContext,
    const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor) {
    uint32_t stride = sizeof(Vertex);
    uint32_t offset = 0;
    immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediateContext->IASetInputLayout(inputLayout.Get());

    immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    Constants data = { world, materialColor };
    immediateContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
    immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

    D3D11_BUFFER_DESC bufferDesc = {};
    indexBuffer->GetDesc(&bufferDesc);
    immediateContext->DrawIndexed(bufferDesc.ByteWidth / sizeof(uint32_t), 0, 0);
}