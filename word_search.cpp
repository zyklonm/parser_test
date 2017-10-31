#include    <iostream>
#include    <fstream>
#include    <string.h>
#include    <locale.h>
#include    <stdlib.h>
#include    <time.h>
#include    "tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

#define WORD_LENGTH_ONE 1
#define WORD_LENGTH_TWO 2
#define WORD_LENGTH_THREE 3
#define WORD_LENGTH_FOUR 4
#define WORD_LENGTH_FIVE 5

#define WORD_UNICODE_MAX 130
#define WORD_NOT_UNICODE_MAX 500

#define INDEX_HASH_3BYTE_WORD 130
char *word;
int xml_num = 0;
int for_i =0;
int word_length;
int len_1;
int len_2;
int len_3;
int len_4;
int len_5;

int total_word_num = 0;

int *sort_num;
int sort_num_n = 0;

int min_num = 999999;
int max_num = -999999;

struct trie_tree
{
    struct trie_tree *children_node[WORD_UNICODE_MAX];

    bool isLastWord;
    unsigned int number_of_value;
    unsigned int checker_num; 
    char *word;
};

struct trie_tree *GetNode()
{
    struct trie_tree *node = new trie_tree;

    node->isLastWord = false;
    node->number_of_value = 0;
    node->checker_num = 0;
    word = NULL;
    int i;
    for( i = 0; i < WORD_UNICODE_MAX; i++ )
    {
	node->children_node[i] = NULL;
    }

    return node;
}

void print_node(struct trie_tree *root)
{
    struct trie_tree *node = root;
    int i;
    for( i =0; i < WORD_UNICODE_MAX; i++)
    {
	if( node->isLastWord == true )
	{
	    cout<<node->word<<"   :    "<<node->number_of_value<<endl;
	    sort_num[sort_num_n] = node->number_of_value;
	    sort_num_n += 1;
	    return;
	}
	else if( node->children_node[i] )
	{
	    print_node( node->children_node[i] );
	}
    }
}
void insert_tree(struct trie_tree *root, char *strText)
{
    struct trie_tree *insert_node = root;

    int size_of_string = strlen(strText);

    int i;
    for( i = 0; i < size_of_string; i++ )
    {
	int location = *(strText+i) + INDEX_HASH_3BYTE_WORD;
	if( location >= max_num )
	{
	    max_num = location;
	}
	if(location <= min_num )
	{
	    min_num = location;
	}
	if( !insert_node->children_node[location] )
	{
	    insert_node->children_node[location] = GetNode();
	}
	insert_node = insert_node->children_node[location];
    }

    insert_node->isLastWord = true;
    if( insert_node->checker_num != xml_num)
    {
	if( insert_node->word == NULL )
	{
	    insert_node->word = new char(size_of_string);
	    for( i = 0; i < size_of_string; i ++ )
	    {
		insert_node->word[i] = *(strText + i);
	    }
	    total_word_num +=1;
	}

	insert_node->word[i] = '\0';
	insert_node->number_of_value += 1;
	insert_node->checker_num = xml_num;
    }
}

int compare(const void *first, const void* second)
{
    int num1 = *(int *)first;
    int num2 = *(int *)second;

    if( num1 < num2)
    {
	return -1;
    }
    else if ( num1 > num2 )
    {
	return 1;
    }
    else
	return 0;
}

void separation(char* strText,int mode, struct trie_tree *root)
{
    for( ; *strText != '\0'; strText++ )
    {
	len_1 = mblen(&strText[0],MB_CUR_MAX);
	len_2 = mblen(&strText[len_1],MB_CUR_MAX);
	len_3 = mblen(&strText[len_1+len_2],MB_CUR_MAX);
	len_4 = mblen(&strText[len_1+len_2+len_3],MB_CUR_MAX);
	len_5 = mblen(&strText[len_1+len_2+len_3+len_4],MB_CUR_MAX);
	if( len_1 > 1 )
	{
	    switch(mode)
	    {
		case WORD_LENGTH_ONE:
		{
		    word = new char(len_1);
		    for( for_i = 0; for_i < len_1; for_i ++ )
		    {
			word[for_i] = strText[for_i];
		    }
		    word[for_i] = '\0';
		    insert_tree(root, word);
		    strText += (len_1-1);
		    break;
		}

		case WORD_LENGTH_TWO:
		{
		    if( len_2 > 1 )
		    {
			word = new char(len_1 + len_2);
			for( for_i = 0; for_i < len_1+len_2; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2-1);
		    }
		    break;
		}

		case WORD_LENGTH_THREE:
		{
		    if( len_2 > 1 && len_3 > 1)
		    {
			word = new char(len_1 + len_2 + len_3);
			for( for_i = 0; for_i < len_1+len_2+len_3; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3-1);
		    }
		    break;
		}

		case WORD_LENGTH_FOUR:
		{
		    if( len_2 > 1 && len_3 > 1 && len_4 > 1)
		    {
                        word = new char(len_1 + len_2 + len_3 + len_4);
			for( for_i = 0; for_i < len_1+len_2+len_3+len_4; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}			                        
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3+len_4-1);
		    }
		    break;
		}

		case WORD_LENGTH_FIVE:
		{
		    if( len_2 > 1 && len_3 > 1 && len_4 > 1 && len_5 >1 )
		    {
                        word = new char(len_1 + len_2 + len_3 + len_4 +len_5);
			
			for( for_i = 0; for_i < len_1+len_2+len_3+len_4+len_5; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3+len_4+len_5-1);
		    }
		    break;
		}

		default:
		{
		    cout<<"Do not Support length of word : " <<mode<<endl;
		    break;
		}
	    }
	}
    }
}

int main()
{
    do
    {
	cout<<"length of the word?"<<endl;
	cin>>word_length;
    }while(word_length == 0 || word_length > WORD_LENGTH_FIVE);
    
    struct trie_tree *root = GetNode();
    clock_t start, end, sub_start, sub_end;
    start = clock();
    
    setlocale( LC_CTYPE, "" );

    XMLDocument xml;
    xml.LoadFile("jp_wiki.xml");

    XMLElement *pageElm = xml.FirstChildElement("page");
    int n = 0;
    int num = 0;
    while( pageElm != NULL )
    {
	xml_num +=1;
	char *title_string = (char*)(pageElm->FirstChildElement("title")->GetText());
	char *category_string = (char*)(pageElm->FirstChildElement("category")->GetText());
	char *text_string = (char*)(pageElm->FirstChildElement("text")->GetText());

	sub_start = clock();	

	separation(text_string, word_length, root);

	sub_end = clock();
	pageElm = (XMLElement *)(pageElm->NextSibling());
	n += 1;
//	cout<<n<<endl;
    }
    cout<<"end"<<endl;
    sort_num = new int(total_word_num+20);
    print_node(root);
    cout<<"sort"<<sort_num_n<<endl;
    end = clock();
    cout<<"Total : " << total_word_num<<endl;
    cout<<"max:"<<max_num<<", min:"<<min_num<<endl;
    printf("Run Time : %.3lf(sec) \n",(float)(end-start)/(CLOCKS_PER_SEC));
   
    qsort(sort_num, sort_num_n, sizeof(int), compare);
    for( for_i = 0; for_i < sort_num_n; for_i ++ )
    {
	cout<<sort_num[for_i]<<endl;
    }
    
    return 0;
}
