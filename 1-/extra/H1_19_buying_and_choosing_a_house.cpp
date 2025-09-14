// 房屋选购系统 - 根据政策优先级排序并查询选房顺序
#include<bits/stdc++.h>
using namespace std;

int available_houses; // 可供申请的房屋套数

struct Applicant
{
    char id_number[19];        // 身份证号
    int social_security_months; // 社保缴纳月数
    int housing_area;          // 自有住房面积
    char application_date[11]; // 申报时间

    // 排序规则：优先级比较
    bool operator <(const Applicant &other) const
    {
        // 不符合购房条件的申请者排在最后
        if (housing_area == 0 && social_security_months <= 24) return false;
        if (other.housing_area == 0 && other.social_security_months <= 24) return true;

        // 刚性需求 vs 改善性需求：刚性需求优先
        if (housing_area != other.housing_area) return housing_area < other.housing_area;

        // 同为刚性需求：社保缴纳月数多者优先
        if (housing_area == 0 && social_security_months != other.social_security_months)
            return social_security_months > other.social_security_months;

        // 申报时间比较：年份优先
        for (int i = 6; i < 10; ++i) {
            if (application_date[i] != other.application_date[i])
                return application_date[i] < other.application_date[i];
        }

        // 月份比较
        for (int i = 0; i < 2; ++i) {
            if (application_date[i] != other.application_date[i])
                return application_date[i] < other.application_date[i];
        }

        // 日期比较
        for (int i = 3; i < 5; ++i) {
            if (application_date[i] != other.application_date[i])
                return application_date[i] < other.application_date[i];
        }

        return false;
    }
};

Applicant *applicants; // 申请者数组

// 从二进制文件读取申请者信息
Applicant *loadApplicantData(int &total_count)
{
    FILE *file_pointer = fopen("house.bin", "rb");

    // 读取文件末尾的总人数
    fseek(file_pointer, -1 * (long)sizeof(int), SEEK_END);
    fread(&total_count, sizeof(int), 1, file_pointer);

    // 回到文件开头读取申请者数据
    rewind(file_pointer);
    Applicant *temp_array = new Applicant[total_count];
    fread(temp_array, sizeof(Applicant), total_count, file_pointer);

    fclose(file_pointer);
    return temp_array;
}

int total_applicants;
map<string, int> id_to_index_map; // 身份证号到数组索引的映射

// 处理单个查询
void processQuery()
{
    char query_id[19];
    cin >> query_id;

    string id_string = query_id;
    int applicant_index = id_to_index_map[id_string];

    // 找到相同排名的区间
    int left_bound = lower_bound(applicants, applicants + total_applicants, applicants[applicant_index]) - applicants + 1;
    int right_bound = upper_bound(applicants, applicants + total_applicants, applicants[applicant_index]) - applicants;

    // 判断输出情况
    if ((applicants[applicant_index].housing_area == 0 && applicants[applicant_index].social_security_months <= 24) ||
        left_bound > available_houses) {
        // 情况1：不符合条件或排名超出房源数量
        cout << "Sorry" << endl;
    }
    else if (left_bound == right_bound) {
        // 情况2：该名次只有1人
        cout << left_bound << endl;
    }
    else if (right_bound <= available_houses) {
        // 情况3：该名次有多人，但都能分到房
        cout << left_bound << " " << right_bound << endl;
    }
    else {
        // 情况4：该名次有多人，但房源不够
        int selected_count = available_houses - left_bound + 1;
        int total_same_rank = right_bound - left_bound + 1;
        cout << selected_count << "/" << total_same_rank << endl;
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    // 读取申请者数据
    applicants = loadApplicantData(total_applicants);

    // 按照政策规则排序
    sort(applicants, applicants + total_applicants);

    // 建立身份证号到索引的映射
    for (int i = 0; i < total_applicants; ++i) {
        string id_string = applicants[i].id_number;
        id_to_index_map[id_string] = i;
    }

    // 读取房源数量和查询次数
    int query_count;
    cin >> available_houses >> query_count;

    // 处理每个查询
    while (query_count--) {
        processQuery();
    }

    return 0;
}