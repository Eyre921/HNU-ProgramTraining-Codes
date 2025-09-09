import os
import re

# --- 配置 ---
# 设置为 False 来真正执行重命名操作。
# 默认为 True，只会打印将要发生的变化，不会修改任何文件。
DRY_RUN = False


# ----------------

def to_snake_case(text):
    """将字符串转换为蛇形命名法（snake_case）"""
    # 1. 在大写字母前插入下划线 (处理 PascalCase 或 camelCase)
    #    例如 "WorldCup" -> "World_Cup"
    s = re.sub(r'(?<!^)(?=[A-Z])', '_', text)

    # 2. 将所有空格和横线替换为下划线
    s = s.replace(' ', '_').replace('-', '_')

    # 3. 移除除了字母、数字和下划线之外的所有特殊字符
    s = re.sub(r'[^\w_]', '', s)

    # 4. 将整个字符串转换为小写
    s = s.lower()

    # 5. 将连续的多个下划线合并为一个
    s = re.sub(r'__+', '_', s)

    return s


def rename_script_in_dir(root_dir):
    """遍历目录并重命名文件"""
    print(f"扫描目录: {root_dir}\n")
    rename_count = 0

    # os.walk 会遍历指定目录下的所有子目录和文件
    for subdir, _, files in os.walk(root_dir):
        # 忽略 .git 或其他隐藏文件夹
        if os.path.basename(subdir).startswith('.'):
            continue

        print(f"--- 正在处理文件夹: {os.path.relpath(subdir, root_dir)} ---")
        for filename in files:
            # 只处理 .cpp 文件
            if not filename.lower().endswith('.cpp'):
                continue

            # 使用正则表达式匹配 "数字-名称.cpp" 或 "数字_名称.cpp" 的格式
            match = re.match(r'^(\d+)[-_](.+?)(?:\.cpp)+$', filename, re.IGNORECASE)

            if not match:
                print(f"  [跳过] '{filename}' (格式不匹配)")
                continue

            num_part = match.group(1)
            name_part = match.group(2)

            # 格式化为两位数字，不足则补零
            new_num = f"{int(num_part):02d}"

            # 将名称部分转换为 snake_case
            new_name_part = to_snake_case(name_part)

            # 组合成新的文件名，使用下划线作为分隔符
            new_filename = f"{new_num}_{new_name_part}.cpp"

            # 如果新旧文件名相同，则无需处理
            if filename == new_filename:
                print(f"  [不变] '{filename}'")
                continue

            old_filepath = os.path.join(subdir, filename)
            new_filepath = os.path.join(subdir, new_filename)

            print(f"  [计划] '{filename}' -> '{new_filename}'")

            if not DRY_RUN:
                try:
                    os.rename(old_filepath, new_filepath)
                    rename_count += 1
                except OSError as e:
                    print(f"  [错误] 重命名 '{filename}' 失败: {e}")

    print("\n-------------------------------------")
    if DRY_RUN:
        print(" dry_run 模式完成。没有文件被真正重命名。")
        print("要执行重命名，请在脚本中将 DRY_RUN 设置为 False。")
    else:
        print(f"操作完成！总共有 {rename_count} 个文件被成功重命名。")
    print("-------------------------------------")


if __name__ == "__main__":
    # 从脚本所在的当前目录开始执行
    current_directory = os.getcwd()
    rename_script_in_dir(current_directory)