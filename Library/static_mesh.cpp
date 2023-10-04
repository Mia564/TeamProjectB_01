#include "static_mesh.h"
#include "shader.h"
#include "texture.h"
#include "misc.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace DirectX;
StaticMesh::StaticMesh(ID3D11Device* device, const wchar_t* objFilename, bool onInvers) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t currentIndex = 0;

    std::vector<XMFLOAT3> positions;
    std::vector<XMFLOAT3> normals;
    std::vector<XMFLOAT2> texcoords;        // UV���W
    std::vector<std::wstring> mtlFilenames; // �}�e���A���t�@�C����

    // OBJ
    std::wifstream fin(objFilename);
    _ASSERT_EXPR(fin, L"'OBJ file not found.");
    wchar_t command[256];
    while (fin) {
        fin >> command;
        if (0 == wcscmp(command, L"v")) { // ���_���ǂݍ���
            float x, y, z;
            fin >> x >> y >> z;
            positions.push_back({ x,y,z });
            fin.ignore(1024, L'\n'); // ���̉��s�܂�1024�������ǂݔ�΂� ���ꂪ���邱�Ƃŗ]�v�ȕ������ǂ܂��ɍς�
        }
        else if (0 == wcscmp(command, L"vn")) { // �@�����ǂݍ���
            float i, j, k;
            fin >> i >> j >> k;
            normals.push_back({ i,j,k });
            fin.ignore(1024, L'\n'); // ���̉��s�܂�1024�������ǂݔ�΂�
        }
        else if (0 == wcscmp(command, L"vt")) { // �e�N�X�`�����W�ǂݍ���(texcoord)
            float u, v;
            fin >> u >> v;
            if (onInvers) texcoords.push_back({ u,1.0f - v }); // onInvers��true�Ȃ�ϊ����āA�ǂݍ���
            else texcoords.push_back({ u,v }); // onInvers��false�Ȃ炻�̂܂܂̒l��ǂݍ���
            fin.ignore(1024, L'\n'); // ���̉��s�܂�1024�������ǂݔ�΂�
        }
        else if (0 == wcscmp(command, L"f")) { // �ʏ��ǂݍ���
            for (size_t i = 0; i < 3; i++) {
                Vertex vertex;
                size_t v, vt, vn; // ���_���/�e�N�X�`�����/�@�����

                fin >> v;
                vertex.position = positions.at(v - 1);

                if (L'/' == fin.peek()) {
                    fin.ignore(1);
                    if (L'/' != fin.peek()) {
                        fin >> vt;
                        vertex.texcoord = texcoords.at(vt - 1);
                    }
                    if (L'/' == fin.peek()) {
                        fin.ignore(1);
                        fin >> vn;
                        vertex.normal = normals.at(vn - 1);
                    }
                }
                vertices.push_back(vertex);
                indices.push_back(currentIndex++);
            }
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"mtllib")) { // �}�e���A���t�@�C���̓ǂݍ���
            wchar_t mtllib[256];
            fin >> mtllib;
            mtlFilenames.push_back(mtllib);
        }
        else if (0 == wcscmp(command, L"usemtl")) {
            wchar_t usemtl[MAX_PATH] = {};
            fin >> usemtl;
            subsets.push_back({ usemtl,static_cast<uint32_t>(indices.size()),0 });
        }
        else {
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    std::vector<Subset>::reverse_iterator iterator = subsets.rbegin();
    iterator->indexCount = static_cast<uint32_t>(indices.size()) - iterator->indexStart;

    for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator) {
        iterator->indexCount = (iterator - 1)->indexStart - iterator->indexStart;
    }

    // MTL
    std::filesystem::path mtlFilename(objFilename); // filesystem::path �t�@�C���V�X�e���̃p�X�������N���X
    mtlFilename.replace_filename(std::filesystem::path(mtlFilenames[0]).filename()); // �p�X�Ɋ܂܂��t�@�C������u��������

    fin.open(mtlFilename);
    //_ASSERT_EXPR(fin, L"'MTL file not found.");

    while (fin) {
        fin >> command;
        if (0 == wcscmp(command, L"map_Kd")) {
            fin.ignore();
            wchar_t map_Kd[256];
            fin >> map_Kd;

            std::filesystem::path path(objFilename);
            path.replace_filename(std::filesystem::path(map_Kd).filename());
            //materials.rbegin()->textureFilename = path;
            materials.rbegin()->textureFilename[0] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump")) {
            fin.ignore();
            wchar_t mapBump[256];
            fin >> mapBump;

            std::filesystem::path path(objFilename);
            path.replace_filename(std::filesystem::path(mapBump).filename());
            materials.rbegin()->textureFilename[1] = path;
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"newmtl")) {
            fin.ignore();
            wchar_t newmtl[256];
            Material material;
            fin >> newmtl;
            material.name = newmtl;
            materials.push_back(material);
        }
        else if (0 == wcscmp(command, L"Ka")) { // Ambient �����̃J���[�����擾
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->ka = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"Kd")) { // Diffuse �A�̃J���[�����擾
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->kd = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
        else if (0 == wcscmp(command, L"Ks")) { // Specular ���ˌ��̃J���[�����擾
            float r, g, b;
            fin >> r >> g >> b;
            materials.rbegin()->ks = { r,g,b,1 };
            fin.ignore(1024, L'\n');
        }
        else {
            fin.ignore(1024, L'\n');
        }
    }
    fin.close();

    if (materials.size() == 0)
    {
        for (const Subset& subset : subsets)
        {
            materials.push_back({ subset.usemtl });
        }
    }

    D3D11_TEXTURE2D_DESC texture2dDesc = {};

    for (Material& material : materials) {

        if (material.textureFilename->empty()) {
            make_dummy_texture(device, materials[0].shaderResourceView[0].GetAddressOf(), 0xFFFFFFFF, 16);
        }
        else {
            load_texture_from_file(device, material.textureFilename[0].c_str(),
                material.shaderResourceView[0].GetAddressOf(), &texture2dDesc);
        }
    }


    create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

    HRESULT hr = S_OK;

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {     
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    const char* fileName = "./Shader/static_mesh_vs.cso";

    create_vs_from_cso(device,fileName, vertexShader.GetAddressOf(), inputLayout.GetAddressOf(),inputElementDesc,ARRAYSIZE(inputElementDesc));

    fileName = "./Shader/static_mesh_ps.cso";

    create_ps_from_cso(device, fileName, pixelShader.GetAddressOf());

    CalculateBoundingBox(vertices);

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Constants);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void StaticMesh::create_com_buffers(ID3D11Device* device, Vertex* vertices, size_t vertexCount,uint32_t* indices, size_t indexCount) {
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

void StaticMesh::render(ID3D11DeviceContext* immediateContext,
    const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& materialColor) {
    uint32_t stride = sizeof(Vertex);
    uint32_t offset = 0;
    immediateContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediateContext->IASetInputLayout(inputLayout.Get());

    immediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    immediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    for (const Material& material : materials) {
        immediateContext->PSSetShaderResources(0, 1, material.shaderResourceView[0].GetAddressOf());
        immediateContext->PSSetShaderResources(1, 1, material.shaderResourceView[1].GetAddressOf());

        Constants data = { world,materialColor };
        XMStoreFloat4(&data.materialColor, XMLoadFloat4(&materialColor) * XMLoadFloat4(&material.kd));
        immediateContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &data, 0, 0);
        immediateContext->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

        for (const Subset& subset : subsets) {
            if (material.name == subset.usemtl) {
                immediateContext->DrawIndexed(subset.indexCount, subset.indexStart, 0);
            }
        }
    }
}

// �o�E���f�B���O�{�b�N�X���v�Z����֐�
void StaticMesh::CalculateBoundingBox(const std::vector<Vertex>& vertices) {
    DirectX::XMFLOAT3 minPoint = vertices[0].position;
    DirectX::XMFLOAT3 maxPoint = vertices[0].position;

    for (const Vertex& vertex : vertices) {
        minPoint.x = std::fminf(minPoint.x, vertex.position.x);
        minPoint.y = std::fminf(minPoint.y, vertex.position.y);
        minPoint.z = std::fminf(minPoint.z, vertex.position.z);

        maxPoint.x = std::fmaxf(maxPoint.x, vertex.position.x);
        maxPoint.y = std::fmaxf(maxPoint.y, vertex.position.y);
        maxPoint.z = std::fmaxf(maxPoint.z, vertex.position.z);
    }

    boundingBox.min = minPoint;
    boundingBox.max = maxPoint;
}