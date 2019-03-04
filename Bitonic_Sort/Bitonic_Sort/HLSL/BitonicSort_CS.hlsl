#include "BitonicSort.hlsli"

#define BITONIC_BLOCK_SIZE 512

groupshared uint shared_data[BITONIC_BLOCK_SIZE];

[numthreads(BITONIC_BLOCK_SIZE, 1, 1)]
void CS(uint3 Gid : SV_GroupID,
    uint3 DTid : SV_DispatchThreadID,
    uint3 GTid : SV_GroupThreadID,
    uint GI : SV_GroupIndex)
{
    // д�빲������
    shared_data[GI] = gData[DTid.x];
    GroupMemoryBarrierWithGroupSync();
    
    // ��������
    for (uint j = gLevel >> 1; j > 0; j >>= 1)
    {
        // ��������  ��ǰ����  �ȽϽ��  ȡֵ���
        //   �ݼ�      С����    <=      (�ϴ�ֵ)�ϴ�������ֵ
        //   �ݼ�      ������    <=      (��Сֵ)�ϴ�������ֵ
        //   ����      С����    <=      (��Сֵ)��С������ֵ
        //   ����      ������    <=      (�ϴ�ֵ)��С������ֵ
        //   �ݼ�      С����    >       (�ϴ�ֵ)��С������ֵ
        //   �ݼ�      ������    >       (��Сֵ)��С������ֵ
        //   ����      С����    >       (��Сֵ)�ϴ�������ֵ 
        //   ����      ������    >       (�ϴ�ֵ)�ϴ�������ֵ
        
        uint smallerIndex = GI & ~j;
        uint largerIndex = GI | j;
        bool isSmallerIndex = (GI == smallerIndex);
        bool isDescending = (bool) (gDescendMask & DTid.x);
        uint result = ((shared_data[smallerIndex] <= shared_data[largerIndex]) == (isDescending == isSmallerIndex)) ?
            shared_data[largerIndex] : shared_data[smallerIndex];
        GroupMemoryBarrierWithGroupSync();

        shared_data[GI] = result;
        GroupMemoryBarrierWithGroupSync();
    }
    
    // ������
    gData[DTid.x] = shared_data[GI];
}
