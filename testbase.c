#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<assert.h>
FILE* out;
//char *(*dic2)[1000][7];
//这是数组指针，我们要的是数组指针的指针数组
struct array_node{
    char* word;
    int count;
};
struct array_node* dict[10000];
int array[10000];
char *array_words[10000];
int array_top = -1;
void clear_rear(char* str)
{
    int len = strlen(str)-1;
    while (len>=0&&!isalpha(str[len--]))
        str[len+1] = 0;
}

#define ALPHABET_SIZE 26
void array_append(char* str)
{
    int len = strlen(str);
    array_top++;
///////////////////////////////////////////////////////////////////////////////////////
    dict[array_top] = (struct array_node*)malloc(sizeof(struct array_node));
    dict[array_top]->word = (char*)malloc(sizeof(char)*len+1);
    assert(memcpy((dict[array_top]->word),str,len+1));
    dict[array_top]->count = 1;
///////////////////////////////////////////////////////////////////////////////////////
    array[array_top] = 1;
    array_words[array_top] = (char*)malloc(sizeof(char)*len+1);
    assert(memcpy((array_words[array_top]),str,len+1));
}
typedef struct trie_node
{
    int index;   // 记录该节点代表的单词的hash
    struct trie_node *children[ALPHABET_SIZE]; // 各个子节点 
}trie_node;

trie_node* create_trie_node()
{
    trie_node* pNode = (trie_node*)malloc(sizeof(trie_node));
    pNode->index = -1;
    for(int i=0; i<ALPHABET_SIZE; ++i)
        pNode->children[i] = NULL;
    return pNode;
}
int trie_search(trie_node* root, char* key)
{
    trie_node* node = root;
    char* p = key;
    if (strlen(key)==1)
    {
        if (root->children[*p-'a']==NULL)
        {
            return -1;
        }
        else if (root->children[*p-'a']!=NULL)
        {
            return root->children[*p-'a']->index;
        }
    }
    
    while(*p!=0 && node!=NULL)
    {
        node = node->children[*p-'a'];
        ++p;
    }
    if(node == NULL&&*p!=0)
        return -1;
    else
        return node->index;
}
void trie_insert(trie_node* root, char* key)
{
    trie_node* node = root;
    char* p = key;
    int flag = 0;

    while(*p)
    {
        if(node->children[*p-'a'] == NULL)
        {
            if(!flag)
            {
                array_append(key);
                flag = 1;
            }
            node->children[*p-'a'] = create_trie_node();
        }
        node = node->children[*p-'a'];
        ++p;
    }
    if(flag)
    {
        node->index  = array_top;
    }
    else if(*p==0&&node->index==-1)
    {
        array_append(key);
        node->index = array_top;
    }
    else
        dict[node->index]->count += 1;
}

void init_array()//给线性词库初始化
{
        for (int i = 0; i < 10000; i++)
    {
        array[i] = 0;
        free(array_words[i]);   
        array_words[i] = NULL;
    }
}

int main()
{
    time_t start,end;
    start = clock();
    trie_node* root = create_trie_node();
    FILE* in = fopen("result.txt","r");
    out = fopen("testbase_out.txt","w");

    char word[90];
    int two_sum = 0;
    int scan_num = 0;
    int s_sum = 0;
    init_array();//Clear array system out, no matter fore garbage exists or not;
    while (fscanf(in,"%s",word)!=-1)
    {
        clear_rear(word);
        if(strlen(word)==strlen("konfessionslosen"))
            {
                if (strcmp(word,"konfessionslosen")==0)
                {
                    s_sum++;
                }
                
                if(trie_search(root,word)!=-1)
                    two_sum ++;
                printf("%s\n",word);

            }
               scan_num++;
        trie_insert(root, word);
       
    }

    char mtarget[] = "konfessionslosen";
    clear_rear(mtarget);
    // for (int i = 0; i < 100000; i++)
    printf("%d %d\n",trie_search(root,mtarget),s_sum);
    printf("%s\n",array_words[trie_search(root,mtarget)]);
    int sum = 0;
    for (int i = 0; i <= array_top; i++)
    {
        fprintf(out,"%s : %d\n",dict[i]->word, dict[i]->count);
        sum+=dict[i]->count;
    }
    printf("sum: %d\n",sum);

    //printf("%d\n",sizeof(dic2));
//    set_up();
    fclose(in);
    fclose(out);
    end = clock();
    printf("Time: %f\n",(double)(end-start)/CLOCKS_PER_SEC);

    //sort compartment
// Myqsort(array_words);
    return 0;      
}

//事实证明FGETS在读取完一行东西后不会把结尾的东西丢掉


//现在来做一个故障排查，我们已经打了补丁使得
// 单个字符长度字符串能够被读入，然鹅现在的问题是
// 检查那个大长串为什么找不到
// 我们的排查方案是先确定是不是没插入，再确定
// 查找的问题

// 第一次测试结果：读入次数记上了，但查找没找到（返回-1）
// konfessionslosen
// konfessionslosen
// -1 2
// sum: 1108
// 通过排查发现插入成功，两个大长串被输出在239位置。
// 进一步排查，发现进行search的时候确实出现了问题，
// konfessionslosen从ko后的n就断掉了，出现空指针。
// 我想我需要整个遍历一遍这个树来看看情况了吧。
// 目前的进一步解决方案是：在insert中的search里，查看
// 当konfessionslosen的n被插入时，是否能够找到ko的位置。
// 这很费时间的啊！但是没办法要做。如果发现插入时候
// 正常可以搜索到而后续再查找时候搜索不到，那么就要
// 考虑内存被破坏的问题了。
//目前看来，正常插入了，很正常的插入。我注意到插入和
//搜索用的不是一套东西，我们来检测一下。