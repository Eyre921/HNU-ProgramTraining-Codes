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
/*
12. 内存管理
【问题描述】

       离第一个操作系统HNU-OS发布已经没有多少时间了，但它的一些组件还没有完成，内存管理器就是其中之一。根据开发人员的计划，在第一个版本中，内存管理器将非常简单和直观。它将支持三个操作：

alloc n —— 分配n个字节内存，返回已分配块的正整数标识符x(x初始值为0，每次分配增长1)

erase x —— 删除标识符x所在的块

defragment —— 整理空余内存碎片，将所有块尽量靠近内存的开始位置，并保持各自的顺序

       在此情况下，内存模型非常简单，它是一个m字节的序列，为了方便起见，从第一个字节到第m字节进行编号。

       第一个操作alloc n有一个参数n，表示被分配的内存块大小。在处理此操作时，内存中将分配n个连续字节的空闲块。 如果这些块的数量超过一个，则优先选择最接近内存开始(即第一个字节)的块。 所有这些字节都被标记为非空闲，内存管理器返回一个32位整数数字令牌，代表该块的标识符。 如果不可能分配这样大小的空闲块，则返回NULL。

       第二个操作erase x以x为参数，表示某个块的标识符。此操作释放系统内存，将此块的字节标记为空闲以供进一步使用。 如果此标识符没有指向先前分配的块(该块尚未被释放)，则返回ILLEGAL_ERASE_ARGUMENT。

       最后一个操作defragment没有任何参数，只会使占用的内存部分更接近内存的开始，而不会更改它们各自的顺序。

       在当前的实现中，将使用从1开始的连续整数作为标识符。每个成功的alloc操作过程都应该返回接下来的编号。不成功的alloc操作不影响计数。

       编写内存管理器的实现，为每个alloc命令输出返回的值，为所有失败的erase命令输出ILLEGAL_ERASE_ARGUMENT。

【输入形式】

       输入数据的第一行包含两个正整数t和m（1<=t<=500, 1<=m<=105)，其中t表示需要内存管理器来处理的操作个数，m表示有效的内存字节大小。接下来的t行每一行代表一个操作。
【输出形式】

       输出有多行，每行或者是alloc操作的结果，或者是失败的erase操作的结果ILLEGAL_ERASE_ARGUMENT。其顺序与输入的操作次序一致。

【样例输入】

6 10
alloc 5
alloc 3
erase 1
alloc 6
defragment
alloc 6

【样例输出】

1
2
NULL
3
 */