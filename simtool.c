#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<time.h>
FILE* ARTICLE_FILE,* STWORD_FILE,* HASH_FILE,*RESULT_FILE,*SAMPLE_FILE;
////////////////////////////////////////trei tree module from testbase1///////////////////
struct array_node{
    char* word;
    int count;
};
int bucket_hammin[4][10005];
int zero_num ,one_num, two_num ,three_num;
struct array_node* dict[106000];
struct array_node* hotkey_dict[10005];
int array_top = -1;
#define ALPHABET_SIZE 26
void array_append(char* str)
{
    int len = strlen(str);
    array_top++;
    dict[array_top] = (struct array_node*)malloc(sizeof(struct array_node));
    dict[array_top]->word = (char*)malloc(sizeof(char)*len+1);
    assert(memcpy((dict[array_top]->word),str,len+1));
    dict[array_top]->count = 1;
}
void hotkey_dict_append(char* str)//代码与array_append冗余，但是为了方便理解，暂时不改
{
    int len = strlen(str);
    array_top++;
    hotkey_dict[array_top] = (struct array_node*)malloc(sizeof(struct array_node));
    hotkey_dict[array_top]->word = (char*)malloc(sizeof(char)*len+1);
    assert(memcpy((hotkey_dict[array_top]->word),str,len+1));
    hotkey_dict[array_top]->count = 1;
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
        node = node->children[*p-'a'];///////////////////////////////////
        if (*(p+1)==0)
        {
            if (node==NULL)
                return -1;
            else if (node!=NULL)
                    return node->index;
        }
        ++p;
    }
    if(node == NULL&&*p!=0)
        return -1;
    else if(node == NULL&&*p==0)
        return node->index;
}
void trie_insert(trie_node* root, char* key,int tag)
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
                if(tag)
                hotkey_dict_append(key);
                else
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
        if(tag)
            hotkey_dict_append(key);
        else
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
        free(dict[i]);
        dict[i] = NULL;
    }
}
int dict_cmp(const void** a, const void** b)
{
    struct array_node* tmpa = *(struct array_node**)a;
    struct array_node* tmpb = *(struct array_node**)b;
    if(tmpa->count<tmpb->count)
        return 1;
    else if(tmpa->count>tmpb->count)
        return -1;
    else
        return strcmp(tmpa->word,tmpb->word);
}
//////////////////////////////////////////////////////////////////////////////////////////
typedef struct node
{
    char* name_code;
    char* fingerprint;
}articleNode;

char *hash[10005];

articleNode *sample;
articleNode *articles[16001];
int article_num = 0;
char *sample_name;

char *stopWord[10000];
int stopWordNum = 0;

int N, M;


void clear_rear(char* str)
{
    int len = strlen(str)-1;
    while (len>=0&&!isalpha(str[len--]))str[len+1] = 0;}
void get_hash()
{char line[200];
    for(int i = 0;i<N;i++){
        memset(line,0,sizeof(line));
        fgets(line,200,HASH_FILE);
                hash[i] = (char*)malloc(sizeof(char)*(M+1));
                memcpy(hash[i],line,M);
                hash[i][M] = 0;
                //fprintf(RESULT_FILE,"%s\n",hash[i]); 
                }
                               
}

void get_stw()
{
    int stopword_limit = 10000;
    char word[90];
    memset(word,0,sizeof(word));
    while (fscanf(STWORD_FILE,"%s",word) != EOF)
    {
        clear_rear(word);
        stopWord[stopWordNum] = (char*)malloc(sizeof(char)*(strlen(word)+1));
        strcpy(stopWord[stopWordNum],word);
        stopWordNum++;
    }
}
int Mybsearch(char **arr,char *target,int len)
{
    int left = 0,right = len-1;
    int mid;
    while(left<=right)
    {
        mid = left+(right-left)/2;
        if(strcmp(arr[mid],target)==0)
        {
            //printf("find it at %d\n",mid);
            return mid;
        }
        else if(strcmp(arr[mid],target)<0)
            left = mid+1;
        else
            right = mid-1;
    }
    return -1;
}
// USAGE:int target_index = Mybsearch(stw,target,stopWordNum);
int get_sample_name()
{
    char line[1024];
    memset(line,0,sizeof(line));
    if(fscanf(SAMPLE_FILE,"%s",line)==EOF)
        return 0;
        int len = strlen(line);
        sample_name = (char*)malloc(sizeof(char)*(len+1));
        memcpy(sample_name,line,len+1);
        fprintf(RESULT_FILE,"%s\n",sample_name);
        return 1;
}
int get_article_FGP(FILE* STREAM, int article_i,trie_node* root,int tag)//代码内部冗余
{
    if(tag==0)//dict mode
    {
    assert(articles[article_i] = (articleNode*)malloc(sizeof(articleNode)));
    assert(articles[article_i]->fingerprint = (char*)malloc(sizeof(char)*(M+1)));
    memset(articles[article_i]->fingerprint,0,sizeof(char)*(M+1));
    char line[1024];
    fscanf(STREAM,"%s",line);
    int len = strlen(line);
    assert(articles[article_i]->name_code = (char*)malloc(sizeof(char)*(len+1)));
    memcpy(articles[article_i]->name_code,line,len+1);
    //printf("%s\n",articles[article_i]->name_code);
    //articles[article_i] name-code get confirmed

    char word[90];
    int word_len = 0;
    memset(word,0,sizeof(word));
    char ch;
    fscanf(STREAM,"%c",&ch);
    while(ch!='\f')
    {
        if (isalpha(ch))
        {
            word_len = 0;
        memset(word,0,sizeof(word));
        word[word_len++] =tolower(ch);
        fscanf(STREAM,"%c",&ch);
        while (isalpha(ch))
        {
            word[word_len++] = tolower(ch);
            fscanf(STREAM,"%c",&ch);       
        }
        word[word_len] = 0;

        int len = strlen(word);
        int tag = Mybsearch(stopWord,word,stopWordNum);
            if(tag==-1)//it is not stw
            {
                trie_insert(root, word,0);
            }
        }
        if(fscanf(STREAM,"%c",&ch)==EOF)
            return 0;
    }//words composing module confirmed
    return 1;
    }
    else//hotkey mode
    {
            char word[90];
        int word_len = 0;
        int weight[10005];
        memset(weight,0,sizeof(int)*10005);
        memset(word,0,sizeof(word));
        char ch;
        fscanf(STREAM,"%c",&ch);
        while(ch!='\f')
        {
            if (isalpha(ch))
            {
            word_len = 0;
            memset(word,0,sizeof(word));
            word[word_len++] =tolower(ch);
            fscanf(STREAM,"%c",&ch);
            while (isalpha(ch))
            {
                word[word_len++] = tolower(ch);
                fscanf(STREAM,"%c",&ch);       
            }
            word[word_len] = 0;
            int len = strlen(word);
            int targ = trie_search(root, word);
                if(targ!=-1)//if found
                    {
                        weight[targ]++;
                    }
            }
            if(fscanf(STREAM,"%c",&ch)==EOF)
                break;
        }//hot words ressuction module confirmed
    
    int sum;
    for(int i = 0; i < M;i++)//Generating fingerprint
    {
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum+= hash[j][i]=='0'?-weight[j]:weight[j];
        }
        articles[article_i]->fingerprint[i] = sum>0?'1':'0';
    }
        return 0;
    }
}
void burn_down(articleNode* article)
{
    int len = strlen(article->name_code);
    for (int i = 0; i < len; i++)
    {
        free(article->name_code);
        article->name_code++;
    }
    article->name_code = NULL;
    len = strlen(article->fingerprint);
    for (int i = 0; i < len; i++)
    {
        free(article->fingerprint);
        article->fingerprint++;
    }
    article->fingerprint = NULL;
    free(article);
    article = NULL;
}

int calc_sample_FGP(trie_node* root)
{
    int successor = 0;
    if (sample_name==NULL)
    {
        if(get_sample_name()==0)
            return 0;
        sample = (articleNode*)malloc(sizeof(articleNode));
        assert(sample->fingerprint = (char*)malloc(sizeof(char)*(M+1)));
        memset(sample->fingerprint,0,sizeof(char)*(M+1));
        assert(sample->name_code = (char*)malloc(sizeof(char)*1024));
        memset(sample->name_code,0,sizeof(char)*1024);
    }
    else
        {
            if(get_sample_name()==0)
                return 0;
            memset(sample->fingerprint,0,sizeof(char)*(M+1));
            memset(sample->name_code,0,sizeof(char)*1024);
        }

    char line[1024];
    fscanf(SAMPLE_FILE,"%s",line);
    int len = strlen(line);
    strcpy(sample->name_code,line);
    //printf("%s\n",sample->name_code);

    char word[90];
    int word_len = 0;
    memset(word,0,sizeof(word));
    int weight[10005];
    memset(weight,0,sizeof(int)*10005);
    char ch;
    fscanf(SAMPLE_FILE,"%c",&ch);
    while(ch!='\f')
    {
        if (isalpha(ch))
        {
            word_len = 0;
        memset(word,0,sizeof(word));
        word[word_len++] =tolower(ch);
        fscanf(SAMPLE_FILE,"%c",&ch);
        while (isalpha(ch))
        {
            word[word_len++] = tolower(ch);
            fscanf(SAMPLE_FILE,"%c",&ch);       
        }
        word[word_len] = 0;
        //int len = strlen(word);//I bet that this is redundant
        int targ = trie_search(root, word);
            if(targ!=-1)//if found
                {
                    weight[targ]++;
                }
        }

        if(fscanf(SAMPLE_FILE,"%c",&ch)==EOF)
            break;
        else if(ch=='\f')
            successor = 1;
    }//hot words ressuction module confirmed

    int sum;
    for(int i = 0; i < M;i++)//Generating fingerprint
    {
        sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum+= hash[j][i]=='0'?-weight[j]:weight[j];
        }
        sample->fingerprint[i] = sum>0?'1':'0';
    }

    int distence = 0;
    zero_num = 0;
    one_num = 0;
    two_num = 0;
    three_num = 0;
    for(int j = 0;j<article_num;j++)
    {
        distence = 0;
        for (int i = 0; i < M; i++)
            {
                if (sample->fingerprint[i]!=articles[j]->fingerprint[i])
                    distence++;
            }
        //fprintf(RESULT_FILE,"第%d号汉明距离: %s %d\n",j,articles[j]->name_code, distence);
        if(distence==0)
            bucket_hammin[0][zero_num++] = j;
        else if(distence==1)
            bucket_hammin[1][one_num++] = j;
        else if(distence==2)
            bucket_hammin[2][two_num++] = j;
        else if(distence==3)
            bucket_hammin[3][three_num++] = j;
    }
    ////////////////////////////////////////////////////这部成功后接下来就要入桶了，看汉明0 1 2 3 的

    if (zero_num)
    {
        fprintf(RESULT_FILE,"0:");
        for (int i = 0; i < zero_num; i++)
            fprintf(RESULT_FILE,"%s ",articles[bucket_hammin[0][i]]->name_code);
        fprintf(RESULT_FILE,"\n");
    }
    if (one_num)
    {
        fprintf(RESULT_FILE,"1:");
        for (int i = 0; i < one_num; i++)
            fprintf(RESULT_FILE,"%s ",articles[bucket_hammin[1][i]]->name_code);
        fprintf(RESULT_FILE,"\n");
    }
    if (two_num)
    {
        fprintf(RESULT_FILE,"2:");
        for (int i = 0; i < two_num; i++)
            fprintf(RESULT_FILE,"%s ",articles[bucket_hammin[2][i]]->name_code);
        fprintf(RESULT_FILE,"\n");
    }
    if (three_num)
    {
        fprintf(RESULT_FILE,"3:");
        for (int i = 0; i < three_num; i++)
            fprintf(RESULT_FILE,"%s ",articles[bucket_hammin[3][i]]->name_code);
        fprintf(RESULT_FILE,"\n");
    }

return successor;
}

int main(int argc, char const *argv[])
{
    time_t start, end;
    start = clock();

    assert(ARTICLE_FILE = fopen("article.txt","r"));
    assert(STWORD_FILE = fopen("stopwords.txt","r"));
    assert(HASH_FILE = fopen("hashvalue.txt","r"));
    assert(RESULT_FILE = fopen("result.txt","w"));///////////////////////////////warning:altering result file
    assert(SAMPLE_FILE = fopen("sample.txt","r"));//fileopen confirmed

    trie_node* root = NULL;
    trie_node* hotkey_root = NULL;
    root = create_trie_node();
    N = atoi(argv[1]);
    M = atoi(argv[2]);//cmd confirmed
    //scanf("%d %d",&N,&M);

    get_hash();//confirmed个毛啊，害我好久。

    get_stw();//confirmed

    //get base article fingerprints
    int not_end;
    do{
        not_end = get_article_FGP(ARTICLE_FILE, article_num,root,0);
        article_num++;
    }while(not_end);

    //排序模块
    qsort(dict,array_top+1,sizeof(struct array_node*),dict_cmp);//偷懒排序


    hotkey_root = create_trie_node();//hotkey_root初始化，大树已熔毁，准备重新构建小树
    int tmp_array_top = array_top;
    array_top = -1;
    for (int i = 0; i < N; i++)//hotkey_root构建
    {
        trie_insert(hotkey_root,dict[i]->word,1);
    }
    
    for (int i = 0; i <= tmp_array_top; i++)//dict垃圾回收
    {
        free(dict[i]->word);
        free(dict[i]);
    }
    //free(dict);
    //还缺大树的垃圾回收

    
    fclose(ARTICLE_FILE);
    ARTICLE_FILE = fopen("article.txt","r");
    for (int i = 0; i < article_num; i++)
    {
        for (int j = 0; j < N; j++)
        {
            hotkey_dict[j]->count = 0;
        }
        get_article_FGP(ARTICLE_FILE, i,hotkey_root,1);
    }
    
    //截止目前，所有的无效内存，如dict,hotkey_dict,trie_tree大树
    //都可以 被回收。唯独不能回收小树，小树还要用。

    sample = NULL;
    sample_name = NULL;
    int has_sample = calc_sample_FGP(hotkey_root);
    printf("%s\n",sample_name);
    if (zero_num)
    {
        printf("0:");
        for (int i = 0; i < zero_num; i++)
        {
            printf("%s ",articles[bucket_hammin[0][i]]->name_code);
        }
        printf("\n");
    }
    if (one_num){
        printf("1:");
        for (int i = 0; i < one_num; i++)
            printf("%s ",articles[bucket_hammin[1][i]]->name_code);
        printf("\n");
    }
    if (two_num){
        printf("2:");
        for (int i = 0; i < two_num; i++)
            printf("%s ",articles[bucket_hammin[2][i]]->name_code);
        printf("\n");
    }
    if (three_num){
        printf("3:");
        for (int i = 0; i < three_num; i++)
            printf("%s ",articles[bucket_hammin[3][i]]->name_code);
        printf("\n");
    }
    //上面的模块是特地为第一个sample在终端里打印准备的
    
    while (has_sample)
    {
        has_sample = calc_sample_FGP(hotkey_root);//滚筒式计算样本指纹，常山之🐍
    }

    fclose(ARTICLE_FILE);
    fclose(STWORD_FILE);
    fclose(HASH_FILE);
    fclose(RESULT_FILE);
    fclose(SAMPLE_FILE);
    end = clock();
    //printf("total time:%f.s\n",(double)(end-start)/CLOCKS_PER_SEC);//可以计时

    return 0;
}