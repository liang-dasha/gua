//"○" 是阴是0，"●"是阳是1
//0为老阴，1为少阴，2为少阳，3为老阳。老阴变爻为阳爻，老阳变爻为阴爻。
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

void f_coin(int a_yao[6])//这个函数被用于投掷硬币
{
    printf("按下空格键以开始投掷硬币\n");
    
    // 每次投掷硬币，循环六次
    for (int i = 0; i < 6; i++)
    {
        while (1)
        {
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) // 检查空格键是否被按下
            {
                printf("投掷第%d次硬币：", i + 1);
                Sleep(500); // 防止重复触发
                                                                                                           
                int add = 0; // 储存每三个硬币的和
                // 投掷三个硬币，并直接输出结果
                for (int j = 0; j < 3; j++)
                {
                    int side = rand() % 2; // 获取随机硬币结果
                    printf(side == 0 ? "○" : "●"); // 输出硬币结果
                    printf(j < 2 ? " " : "\n"); // 每三个换一行
                    add += side; // 累加结果
                }               
                a_yao[i] = add; // 将每次投掷的结果保存到数组
                break; // 退出循环，继续下一次投掷
            }
            Sleep(50); // 降低CPU占用
        }
    }
}

void f_yao(int *p_a_yao)//这个函数被用于画爻
{
    //保存指针原始位置
    int *original_position = p_a_yao;//这是一个指针变量，指向n_yao数组首元素

    p_a_yao+=5; // 指针指向第六个元素
    int cut=0;//记录指针位置
    for (int i = 5; i >=0; i--) //输出未变卦时的卦象
    {
        printf("%6s", " "); // 输出6个空格用于右对齐
        switch (*p_a_yao) {
    case 0: // 老阴
    case 1: // 少阴
        printf("——  —— ");// 输出阴爻
        if (*p_a_yao == 0) {
            printf("X"); // 老阴输出 X
        }
        printf("\n");
        break;
    case 2: // 少阳
    case 3: // 老阳
        printf("—————— "); // 输出阳爻
        if (*p_a_yao == 3) {
            printf("O"); // 老阳输出 O
        }
        printf("\n");
        break;
}
        p_a_yao--; // 指针指向上一个元素
    }

    // 复位指针
    p_a_yao = original_position; // 将指针复位到数组的起始位置
}

void f_print_gua(int *p_a_yao_f_print_gua, const char *a_gua[8][8]) //这个函数用于输出卦象
{
    /*成卦原理：将下卦（从底下往上数的前三爻，倒一是百位，倒二是十位，倒三是个位）
    与上卦（从上往下数的前三爻，倒一是百位，倒二是十位，倒三是个位），然后将其分别十进制化，
    比如下卦是101，就把它十进制化成7，7对应的卦是乾，上卦是010，就把它十进制化成2，2对应的卦是坎。
    这个卦就是坎乾卦，也就是水天需卦。
    */
    int top = 0;
    int bottom = 0;
    bottom = (*p_a_yao_f_print_gua) * 4 + *(p_a_yao_f_print_gua + 1) * 2 + *(p_a_yao_f_print_gua + 2) * 1;
    top = *(p_a_yao_f_print_gua + 3) * 4 + *(p_a_yao_f_print_gua + 4) * 2 + *(p_a_yao_f_print_gua + 5) * 1;
    printf("这是%10s卦\n", a_gua[bottom][top]);
}

void f_gua( int a_yao_main[6],const char *a_gua[8][8])//这个函数用于计算原始卦象
{//第二个参数没有被用到，但是它是这个函数调用的那个函数需要用到的参数，所以我把它写上了。
    int a_yao_f_gua[6]={0};
    int *p_a_yao_f_gua=a_yao_f_gua; // 指针变量，指向n_yao_f_gua数组首元素
    for (int i = 0; i < 6; i++) 
    {
        a_yao_f_gua[i] = ((a_yao_main[i])>1);
    }
    f_print_gua(p_a_yao_f_gua,a_gua);
}

void f_biangua(int a_bianyao_main[6],const char *a_gua[8][8])//这个函数用于计算变卦卦象
{//第二个参数没有被用到，但是它是这个函数调用的那个函数需要用到的参数，所以我把它写上了。
    //传入的数组都-1
    int a_bianyao_f_biangua[6]={0};
    int *p_a_bianyao_f_biangua=a_bianyao_f_biangua; // 指针变量，指向n_bianyao_f_biangua数组首元素
    for (int i = 0; i < 6; i++) 
    {
        a_bianyao_f_biangua[i] = (a_bianyao_main[i]-1);
    }
    f_print_gua(p_a_bianyao_f_biangua,a_gua);
}

void f_four_dongyao(int a_yingyang[6], int a_mark[6]) // 这个函数被用来计算当动爻为四的情况,很复杂，另开一个函数
{
    // 当动爻数为4时，用阳静爻爻辞，如果两动爻同阴同阳，用下静爻爻辞。
    // 动爻数-1才是真正的动爻位置。

    // 求和
    int sum = 0;
    int mark = 0;
    
    for (int j = 0; j < 6; j++)
    {
        sum += a_mark[j];
    }
    
    // 遍历动爻位置，找到第一个静爻
    for (int k = 0; k < 6; k++)
    {
        if (a_mark[k] != k + 1)
        {
            mark = k + 1;
            break;
        }
    }

    // 判断用爻输出用爻
    if (a_yingyang[mark - 1] == a_yingyang[21 - sum - mark - 1]) {
        printf("动爻数为4，两静爻同阴同阳，用变卦下静爻（第%d爻）爻辞。\n", mark);
    } else {
        int selected_mark = (a_yingyang[21 - sum - mark - 1] == 1) ? (21 - sum - mark) : mark;
        printf("动爻数为4，用变卦阳静爻（第%d爻）爻辞。\n", selected_mark);
    }
}

void f_yao_use(int *p_a_yao) // 计算动爻数并给出用爻
{
    int cut=0;//记录动爻数
    int sum=0;
    int a_dongyao_mark[6]={0};//记录动爻位置
    int a_yingyang[6]={-1};//记录阴阳
    int *original_position = p_a_yao; // 保存指针原始位置

    for (int j = 0; j < 6; j++)
    {
        // 计算动爻数,记录动爻位置
        if (*p_a_yao==0||*p_a_yao==3)//如果是老阴或老阳，则为动爻
        {
            a_dongyao_mark[cut]=j+1;
            cut++;
        }

        // 计算阴阳
        if (*p_a_yao==0||*p_a_yao==2)//如果是老阴或少阳，则为阳爻
        {
            a_yingyang[j]=1;
        }else//如果是少阴或老阳，则为阴爻
        {
            a_yingyang[j]=0;
        }
        p_a_yao++; // 指针指向下一个元素
    }
    p_a_yao = original_position; // 复位指针

    // 输出用爻
    switch (cut)
    {
    case 0: //当动爻数为0时，用本卦卦辞
        printf("动爻数为0，用本卦卦辞。\n");
        break;
    case 1://当动爻数为1时，用动爻爻辞
        printf("动爻数为1，用本卦第%d爻爻辞。\n",a_dongyao_mark[0]);
        break;
    case 2://当动爻数为2时，用老阴（阳爻）爻辞，如果两动爻同阴同阳，用上动爻爻辞
        if (a_yingyang[a_dongyao_mark[0]-1]==a_yingyang[a_dongyao_mark[1]-1])
        {
            printf("动爻数为2，用本卦上动爻（第%d爻）爻辞。\n",a_dongyao_mark[1]);//如果两动爻同阴同阳，用上动爻爻辞
        }
        else
        {
            printf("动爻数为2，用本卦阳动爻（第%d爻）爻辞。\n",a_yingyang[a_dongyao_mark[0]-1]==1?a_dongyao_mark[0]:a_dongyao_mark[1]);
            //如果两动爻不同阴阳，用阳爻爻辞，这里判断了下动爻的阴阳，根据判断结果选择用哪个动爻
        }
        break;
    case 3://当动爻数为3时，用中间的动爻爻辞。
        printf("动爻数为3，用本卦第%d爻爻辞\n",a_dongyao_mark[1]);
        break;
    case 5://当动爻数为5时，用静爻爻辞。
        for (int i = 0; i < 6; i++)
        {
            sum+=a_dongyao_mark[i];
        }
        printf("动爻数为5，用变卦的不变爻（第%d爻）爻辞\n",21-sum);
        break;
    case 6:
        printf("动爻数为6，如果是乾坤两卦，用本卦的用爻断之。非乾坤两卦，用变卦卦辞。\n");
        printf("这里给出乾坤的用爻：\n乾·用九：见群龙无首，吉。\n坤·用六：利永贞。\n");
        break;
    default://这是四动爻的情况，因为要调用另一个函数，所以我把它放在这里。
        f_four_dongyao(a_yingyang,a_dongyao_mark);//当动爻数为4时，用阳静爻爻辞，如果两动爻同阴同阳，用下静爻爻辞。
        break;
    }
    
   
}
 

int main() 
{
    SetConsoleOutputCP(65001);// 设置控制台输出为UTF-8编码
    srand(time(0));//初始化随机数种子

    int a_yao[6]={-1};
    int a_bianyao[6]={-1};
    int *p_a_yao=a_yao; // 指针变量，指向n_yao数组首元素
    int *p_a_bianyao=a_bianyao; // 指针变量，指向n_bianyao数组首元素
    const char *a_gua[8][8]= 
{
    {"坤为地", "山地剥", "水地比", "风地观", "雷地豫", "火地晋", "泽地萃", "天地否"},
    {"地山谦", "艮为山", "水山蹇", "风山渐", "雷山小过", "火山旅", "泽山咸", "天山遯"},
    {"地水师", "山水蒙", "坎为水", "风水涣", "雷水解", "火水未济", "泽水困", "天水讼"},
    {"地风升", "山风蛊", "水风井", "巽为风", "雷风恒", "火风鼎", "泽风大过", "天风姤"},
    {"地雷复", "山雷颐", "水雷屯", "风雷益", "震为雷", "火雷噬嗑", "泽雷随", "天雷无妄"},
    {"地火明夷", "山火贲", "水火既济", "风火家人", "雷火丰", "离为火", "泽火革", "天火同人"},
    {"地泽临", "山泽损", "水泽节", "风泽中孚", "雷泽归妹", "火泽睽", "兑为泽", "天泽履", },
    {"地天泰", "山天大畜", "水天需", "风天小畜", "雷天大壮", "火天大有", "泽天夬", "乾为天",},
};

    char choice; // 用于询问用户是否继续

    do{
    f_coin(a_yao); // 开始投掷硬币
    printf("\n硬币投掷完成,开始取爻成卦。\n\n");

    printf("本卦卦象：\n");
    f_yao(p_a_yao);
    f_gua(a_yao,a_gua);

    for (int i = 0; i < 6; i++) // 变卦
    {
        a_bianyao[i] = ((a_yao[i]+1)%2)+1;
    }
    
    printf("\n\n变卦卦象： \n");
    f_yao(p_a_bianyao);
    f_biangua(a_bianyao,a_gua);

    f_yao_use(p_a_yao); //给出用爻



    // 询问用户是否继续
    printf("\n是否继续下一次卜卦？(y/n): ");
    scanf(" %c", &choice); // 读取用户输入，注意前面的空格，如果不加空格，scanf 函数会在遇到空格时，停止读取，导致输入无效。
    } 
    while (choice == 'y' || choice == 'Y'); // 只要用户输入'y'或'Y'，就循环

    return 0;
}
