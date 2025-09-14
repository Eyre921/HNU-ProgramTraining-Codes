/*
8. 恺撒Caesar密码
【问题描述】

Julius Caesar 生活在充满危险和阴谋的年代。为了生存，他首次发明了密码，用于军队的消息传递。假设你是Caesar 军团中的一名军官，需要把Caesar 发送的消息破译出来、并提供给你的将军。消息加密的办法是：对消息原文中的每个字母，分别用该字母之后的第5个字母替换（例如：消息原文中的每个字母A都分别替换成字母F），其他字符不 变，并且消息原文的所有字母都是大写的。 密码字母：A B C D E F G H I J K L M N O P Q R S T U V W X Y Z 原文字母：V W X Y Z A B C D E F G H I J K L M N O P Q R S T U

【输入形式】

最多不超过100个数据集组成。每个数据集由3部分组成：起始行：START 密码消息：由1到200个字符组成一行，表示Caesar发出的一条消息结束行：END 在最后一个数据集之后，是另一行：ENDOFINPUT

【输出形式】

每个数据集对应一行，是Caesar 的原始消息。

【样例输入】

START
NS BFW, JAJSYX TK NRUTWYFSHJ FWJ YMJ WJXZQY TK YWNANFQ HFZXJX
END
START
N BTZQI WFYMJW GJ KNWXY NS F QNYYQJ NGJWNFS ANQQFLJ YMFS XJHTSI NS WTRJ
END
START
IFSLJW PSTBX KZQQ BJQQ YMFY HFJXFW NX RTWJ IFSLJWTZX YMFS MJ
END
ENDOFINPUT
【样例输出】

IN WAR, EVENTS OF IMPORTANCE ARE THE RESULT OF TRIVIAL CAUSES
I WOULD RATHER BE FIRST IN A LITTLE IBERIAN VILLAGE THAN SECOND IN ROME
DANGER KNOWS FULL WELL THAT CAESAR IS MORE DANGEROUS THAN HE
*/
/*
 * 核心思路:
 * 1. 模拟密码破译: 本题要求破译一个恺撒密码。加密规则是将每个大写字母替换为其后的第5个字母。
 * 因此，解密规则就是将每个大写字母替换为其前的第5个字母。
 * 2. 字符处理:
 * a. 遍历加密消息中的每一个字符。
 * b. 判断字符是否为大写英文字母 ('A' 到 'Z')。
 * c. 如果不是（如空格、逗号等），则该字符保持不变，直接附加到结果中。
 * d. 如果是，则执行解密操作，即将其ASCII码减5。
 * 3. 处理循环(Wrapping): 当一个字母向前移动5位时，可能会“越过”字母'A'的边界。例如，解密 'C'，
 * 向前5位是 'B', 'A', 'Z', 'Y', 'X'。正确结果应为 'X'。这可以通过一个简单的条件判断实现：
 * 如果 `字符 - 5` 的结果小于 'A' 的ASCII码，就给它加上26（字母表长度），使其从字母表的末尾“回绕”。
 * 例如，对于 'C'，('C' - 5) 会小于 'A'，所以 ('C' - 5 + 26) 就会得到 'X' 的正确ASCII码。
 * 4. 输入格式处理: 程序需要能够处理由 "START", "END", "ENDOFINPUT" 构成的多数据集格式。
 * 主循环持续读取输入行，直到遇到 "ENDOFINPUT"。在循环内部，当读到 "START" 时，就读取下一行作为
 * 密文进行处理，然后读取并忽略对应的 "END" 行。
 *
 * 时间复杂度: O(N)
 * - 其中 N 是所有数据集中密文的总字符数。
 * - 程序需要遍历每一个密文字符一次来完成解密，因此时间复杂度与总输入大小成线性关系。
 *
 * 空间复杂度: O(L)
 * - 其中 L 是单个数据集中最长密文的长度。
 * - 程序需要一个额外的字符串来存储解密后的结果，其大小与输入的密文长度成正比。
 */

#include <iostream>
#include <string>
#include <vector>

// 使用 std 命名空间，可以省略 `std::` 前缀，使代码更简洁
using namespace std;

/**
 * @brief 对单行恺撒密码密文进行解密
 * @param encryptedText 加密的字符串
 * @return 解密后的原始字符串
 */
string decryptCaesarCipher(const string& encryptedText) {
    string decryptedText = "";

    // 为结果字符串预先分配内存，可以避免在循环中因字符串增长导致的多次内存重新分配，
    // 这是一个简单而有效的性能优化技巧。
    decryptedText.reserve(encryptedText.length());

    // 使用基于范围的 for 循环遍历加密文本中的每一个字符，代码更现代、更安全。
    for (char c : encryptedText) {
        // 检查当前字符是否为大写英文字母
        if (c >= 'A' && c <= 'Z') {
            // 是大写字母，执行解密操作：向前移动5个位置
            char decryptedChar = c - 5;

            // 处理字母表回绕（wrapping）的情况
            if (decryptedChar < 'A') {
                // 如果结果小于'A'，则加上26（字母表长度），使其从'Z'向前回绕
                decryptedChar += 26;
            }
            decryptedText += decryptedChar;
        } else {
            // 如果不是大写字母（例如空格、标点符号），则保持原样
            decryptedText += c;
        }
    }
    return decryptedText;
}

/**
 * @brief 主处理函数，负责读取和控制数据集流程
 */
void solve() {
    string currentLine;

    // 循环读取每一行，直到遇到 "ENDOFINPUT" 标志或输入流结束
    while (getline(cin, currentLine) && currentLine != "ENDOFINPUT") {
        // 一个新的数据集以 "START" 开始
        if (currentLine == "START") {
            // 读取真正的加密消息行
            string encryptedMessage;
            getline(cin, encryptedMessage);

            // 调用解密函数处理消息
            string originalMessage = decryptCaesarCipher(encryptedMessage);

            // 输出解密后的原始消息
            cout << originalMessage << endl;

            // 读取并忽略数据集的 "END" 标志行。
            // 题目保证了格式的严格性，所以我们不需要对这一行的内容做检查。
            getline(cin, currentLine);
        }
    }
}

int main() {
    // 禁用 C++ 标准流与 C 标准流的同步，可以大幅提高 I/O 效率
    ios_base::sync_with_stdio(false);
    // 解除 cin 与 cout 的绑定，避免不必要的刷新，进一步提速
    cin.tie(NULL);

    // 调用解决问题的函数
    solve();

    return 0;
}