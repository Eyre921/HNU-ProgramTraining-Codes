// Original Filename: 12.内存管理.cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

class MemoryManager
{
private:
    vector<bool> memory; // 内存状态，true表示已占用
    map<int, pair<int, int> > allocated_blocks; // 标识符 -> (起始位置, 大小)
    int next_id; // 下一个分配的标识符
    int memory_size;

public:
    MemoryManager(int m) : memory(m, false), next_id(1), memory_size(m) {}

    // 分配n字节内存
    string alloc(int n)
    {
        // 寻找第一个足够大的连续空闲块
        for (int i = 0; i <= memory_size - n; i++)
        {
            bool can_allocate = true;
            for (int j = i; j < i + n; j++)
            {
                if (memory[j])
                {
                    can_allocate = false;
                    break;
                }
            }

            if (can_allocate)
            {
                // 分配内存
                for (int j = i; j < i + n; j++)
                {
                    memory[j] = true;
                }

                // 记录分配信息
                allocated_blocks[next_id] = make_pair(i, n);
                int current_id = next_id;
                next_id++;

                return to_string(current_id);
            }
        }

        return "NULL";
    }

    // 释放标识符为x的内存块
    string erase(int x)
    {
        if (allocated_blocks.find(x) == allocated_blocks.end())
        {
            return "ILLEGAL_ERASE_ARGUMENT";
        }

        pair<int, int> block_info = allocated_blocks[x];
        int start = block_info.first;
        int size = block_info.second;

        // 释放内存
        for (int i = start; i < start + size; i++)
        {
            memory[i] = false;
        }

        // 删除记录
        allocated_blocks.erase(x);

        return ""; // erase成功时不输出
    }

    // 整理内存碎片
    void defragment()
    {
        vector<pair<int, pair<int, int> > > blocks_to_move; // (标识符, (原始位置, 大小))

        // 收集所有已分配的块信息
        for (auto &block: allocated_blocks)
        {
            blocks_to_move.push_back(make_pair(block.first, block.second));
        }

        // 按原始位置排序以保持顺序
        sort(blocks_to_move.begin(), blocks_to_move.end(),
             [](const pair<int, pair<int, int> > &a, const pair<int, pair<int, int> > &b)
             {
                 return a.second.first < b.second.first;
             });

        // 清空内存
        fill(memory.begin(), memory.end(), false);
        allocated_blocks.clear();

        // 重新紧凑分配
        int current_pos = 0;
        for (auto &block: blocks_to_move)
        {
            int id = block.first;
            int size = block.second.second;

            // 在current_pos位置分配
            for (int i = current_pos; i < current_pos + size; i++)
            {
                memory[i] = true;
            }

            // 更新记录
            allocated_blocks[id] = make_pair(current_pos, size);
            current_pos += size;
        }
    }
};

int main()
{
    int t, m;
    cin >> t >> m;

    MemoryManager manager(m);

    for (int i = 0; i < t; i++)
    {
        string operation;
        cin >> operation;

        if (operation == "alloc")
        {
            int n;
            cin >> n;
            string result = manager.alloc(n);
            cout << result << endl;
        } else if (operation == "erase")
        {
            int x;
            cin >> x;
            string result = manager.erase(x);
            if (!result.empty()) cout << result << endl;
        } else if (operation == "defragment")
        {
            manager.defragment();
        }
    }

    return 0;
}
