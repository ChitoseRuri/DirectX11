Buffer<uint> gInput : register(t0);
RWBuffer<uint> gData : register(u0);

cbuffer CB : register(b0)
{
    uint gLevel;        // 2^��Ҫ��������
    uint gDescendMask;  // �½���������
    uint gMatrixWidth;  // ������(Ҫ����>=�߶��Ҷ�Ϊ2�ı���)
    uint gMatrixHeight; // ����߶�
}