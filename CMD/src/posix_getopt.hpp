#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
argc: main 函数的 argc
argv: main 函数的 argv
optstring: 格式控制符。"ab:c:d"代表 -b 和 -b 后面必须跟一个参数，而 -a 和 -d 不需要参数

int getopt(int argc, char * const argv[], const char *optstring);

extern char *optarg; 表示参数的具体内容
extern int optind; 表下一个将被处理到的参数在 argv 中的下标值
extern int opterr;
extern int optopt;


int getopt(int argc, char * const argv[], const char *optstring, const struct option *long_opts, int *longindex);
*/
void testGetOpt(int argc, char *argv[]) 
{
    int opt;  
    const char *optstring = "a:b:c:d"; // 设置短参数类型及是否需要参数

    while ((opt = getopt(argc, argv, optstring)) != -1) 
    {
        printf("opt = %c\n", opt);  // 命令参数，亦即 -a -b -c -d
        printf("optarg = %s\n", optarg); // 参数内容
        printf("optind = %d\n", optind); // 下一个被处理的下标值
        printf("argv[optind - 1] = %s\n\n",  argv[optind - 1]); // 参数内容
    }
}

#include <getopt.h>
/*
argc: main 函数的 argc
argv: main 函数的 argv
optstring: 格式控制符
longopts: 一个由option结构体组成的数组，数组的每个元素，指明了一个“长参数”（即形如–name的参数）名称和性质
longindex: 如果longindex非空，它指向的变量将记录当前找到参数符合longopts里的第几个元素的描述，即是longopts的下标值

int getopt(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
*/

void testGetOptLong(int argc, char *argv[]) 
{
    int opt; // getopt_long() 的返回值
    int digit_optind = 0; // 设置短参数类型及是否需要参数

    // 如果option_index非空，它指向的变量将记录当前找到参数符合long_opts里的
    // 第几个元素的描述，即是long_opts的下标值
    int option_index = 0;
    // 设置短参数类型及是否需要参数
    const char *optstring = "ab:nr:";  

    // 设置长参数类型及其简写，比如 --reqarg <==>-r
    /*
    struct option {
             const char * name;  // 参数的名称
             int has_arg; // 是否带参数值，有三种：no_argument， required_argument，optional_argument
             int * flag; // 为空时，函数直接将 val 的数值从getopt_long的返回值返回出去，
                     // 当非空时，val的值会被赋到 flag 指向的整型数中，而函数返回值为0
             int val; // 用于指定函数找到该选项时的返回值，或者当flag非空时指定flag指向的数据的值
        };
    其中：
        no_argument(即0)，表明这个长参数不带参数（即不带数值，如：--name）
            required_argument(即1)，表明这个长参数必须带参数（即必须带数值，如：--name Bob）
            optional_argument(即2)，表明这个长参数后面带的参数是可选的，（即--name和--name Bob均可）
     */
    static struct option long_options[] = {
        {"reqarg", required_argument, NULL, 'r'},
        {"noarg",  no_argument,       NULL, 'n'},
        {"optarg", optional_argument, NULL, 'o'},
        {0, 0, 0, 0}  // 添加 {0, 0, 0, 0} 是为了防止输入空值
    };

    while ( (opt = getopt_long(argc,
                               argv,
                               optstring,
                               long_options,
                               &option_index)) != -1) {
        printf("opt = %c\n", opt); // 命令参数，亦即 -a -b -n -r
        printf("optarg = %s\n", optarg); // 参数内容
        printf("optind = %d\n", optind); // 下一个被处理的下标值
        printf("argv[optind - 1] = %s\n",  argv[optind - 1]); // 参数内容
        printf("option_index = %d\n", option_index);  // 当前打印参数的下标值
        printf("\n");
    }
}
