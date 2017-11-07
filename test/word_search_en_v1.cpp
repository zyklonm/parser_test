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

char *word;
char *search_word;
int xml_num = 0;
int for_i =0;
int word_length;
int len_1;
int len_2;
int len_3;
int len_4;
int len_5;

int *sort_num;
int sort_num_n = 0;

int min_num = 999999;
int max_num = -999999;

struct trie_tree
{
    struct trie_tree *next_node;
    struct trie_tree *children_node;

    bool isLastWord;
    unsigned int number_of_value;
    unsigned int checker_num; 
    char single_word;
};


struct trie_tree *CreateNode()
{
    struct trie_tree *node = new trie_tree;

    node->isLastWord = false;
    node->number_of_value = 0;
    node->checker_num = 0;
    node->single_word = '\0';
    
    node->children_node = NULL;
    node->next_node = NULL;

    return node;
}

void print_node(struct trie_tree *node, int depth)
{
    if( node->children_node != NULL )
    {
	if( node->single_word != '\0' )
	{
	    *(search_word + depth) = node->single_word;
	    depth += 1;
	}
	print_node(node->children_node,depth);
	depth -= 1;
    }

    if( node->next_node != NULL )
    {
	if( node->single_word != '\0' )
	{
	    *(search_word + depth) = node->single_word;
	}
	print_node(node->next_node, depth);
    }

    if( node->isLastWord )
    {
	*(search_word + depth) = '\0' ;
	cout<<search_word<<" : "<<node->number_of_value<<endl;
    }
}

void insert_tree(struct trie_tree *root, char *strText)
{
    struct trie_tree *insert_node = root->children_node;
    
    int size_of_string = strlen(strText);
    int i;

    // first insert at root
    if( insert_node == NULL )
    {
	insert_node = root;
	for( i =0; i< size_of_string; i ++ )
	{
	    insert_node->children_node = CreateNode();
	    insert_node = insert_node->children_node;

	    insert_node->single_word = *(strText + i );
	}

	insert_node->children_node = CreateNode();
	insert_node = insert_node->children_node;

	insert_node->isLastWord = true;
	
	insert_node->number_of_value += 1;
	insert_node->checker_num = xml_num;

	return ;
    }

    for( i = 0; i < size_of_string; i++ )
    {
	if( insert_node->single_word == *(strText + i ) )
	{
	    if( insert_node->children_node == NULL )
		insert_node->children_node = CreateNode();
	    insert_node = insert_node->children_node;
	}
	else
	{
	    bool while_checker = true;
	    while( while_checker )
	    {
		if( insert_node->single_word == *(strText + i) )
		{
		    if( insert_node->children_node == NULL )
			insert_node->children_node = CreateNode();
		    insert_node = insert_node->children_node;
		    while_checker = false;
		}
		else if( insert_node->next_node == NULL )
    		{
    		    insert_node->next_node = CreateNode();
    		    insert_node = insert_node->next_node;
    		    insert_node->single_word = *(strText + i);

		    insert_node->children_node = CreateNode();
		    insert_node = insert_node->children_node;
		    while_checker = false;
    		}
    		else
    		{
    		    insert_node = insert_node->next_node;
    		}
	    }
	}
    }

    insert_node->isLastWord = true;
    if( insert_node->checker_num != xml_num)
    {
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
	if( len_1 == 1 )
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
		    free(word);
		    break;
		}

		case WORD_LENGTH_TWO:
		{
		    if( len_2 == 1 )
		    {
			word = new char(len_1 + len_2);
			for( for_i = 0; for_i < len_1+len_2; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2-1);
			free(word);
		    }
		    break;
		}

		case WORD_LENGTH_THREE:
		{
		    if( len_2 == 1 && len_3 == 1)
		    {
			word = new char(len_1 + len_2 + len_3);
			for( for_i = 0; for_i < len_1+len_2+len_3; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3-1);
			free(word);
		    }
		    break;
		}

		case WORD_LENGTH_FOUR:
		{
		    if( len_2 == 1 && len_3 == 1 && len_4 == 1)
		    {
                        word = new char(len_1 + len_2 + len_3 + len_4);
			for( for_i = 0; for_i < len_1+len_2+len_3+len_4; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}			                        
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3+len_4-1);
			free(word);
		    }
		    break;
		}

		case WORD_LENGTH_FIVE:
		{
		    if( len_2 == 1 && len_3 == 1 && len_4 == 1 && len_5 == 1 )
		    {
                        word = new char(len_1 + len_2 + len_3 + len_4 +len_5);
			
			for( for_i = 0; for_i < len_1+len_2+len_3+len_4+len_5; for_i ++)
			{
			    word[for_i] = strText[for_i];
			}
			word[for_i] = '\0';
			insert_tree(root, word);
			strText += (len_1+len_2+len_3+len_4+len_5-1);
			free(word);
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
    
    struct trie_tree *root = CreateNode();
    clock_t start, end, sub_start, sub_end;
    start = clock();
    
    setlocale( LC_CTYPE, "" );

    XMLDocument xml;
    xml.LoadFile("en_wiki.xml");

    XMLElement *pageElm = xml.FirstChildElement("page");
    int n = 0;
    int num = 0;
    while( pageElm != NULL )
    {
	xml_num +=1;
	char *title_string = (char*)(pageElm->FirstChildElement("title")->GetText());
	char *category_string = (char*)(pageElm->FirstChildElement("category")->GetText());
	char *text_string = (char*)(pageElm->FirstChildElement("text")->GetText());
	if ( text_string != NULL )
	{
		sub_start = clock();	

		separation(text_string, word_length, root);
		sub_end = clock();
	}
	pageElm = (XMLElement *)(pageElm->NextSibling());
    }
    cout<<"end"<<endl;
    search_word = new char(20);
    print_node(root,0);
    end = clock();
    printf("Run Time : %.3lf(sec) \n",(float)(end-start)/(CLOCKS_PER_SEC));
    cout<<"max:"<<max_num<<", min:"<<min_num<<endl;
   
    
    return 0;
}
