#include    <iostream>
#include    <fstream>
#include    <string>
#include    <stdio.h>
#include    <stdlib.h>
#include    <locale.h>
#include    "tinyxml2/tinyxml2.h"

#define category_len 11
#define TEXT_MAX_LENGTH 2000

char *modify_text = NULL;
char *category_text = NULL;

char *word_separation_text[100];
int *word_separation_length;

int word_separation_max = 0;
using namespace std;


void delete_enter(char* strText)
{
    for(; *strText != '\0'; strText++)
    {
	if(*strText == '\n')
	    *strText = '.';
    }
}

void parsing_category(char *strText)
{
    int i_num;
    int category_text_length = 0;
    bool checker_category_text = false;

    category_text = (char*)malloc(sizeof(char)*TEXT_MAX_LENGTH);

    for( i_num = 0; i_num <= TEXT_MAX_LENGTH; i_num++ )
    {
	*(category_text+i_num) ='\0';
    }

    for( ; *strText != '\0'; strText ++ )
    {
	if( category_text_length >= TEXT_MAX_LENGTH )
	{
	    break;
	}
	if( *(strText + 0 ) == '[' &&
	    *(strText + 1 ) == '[' &&
	    *(strText + 2 ) == 'C' &&
	    *(strText + 3 ) == 'a' &&
	    *(strText + 4 ) == 't' &&
	    *(strText + 5 ) == 'e' &&
	    *(strText + 6 ) == 'g' &&
	    *(strText + 7 ) == 'o' &&
	    *(strText + 8 ) == 'r' &&
	    *(strText + 9 ) == 'y' &&
	    *(strText + 10 ) == ':' )
	{
	    checker_category_text = true;
	    strText += 11;
	}

	if( checker_category_text )
	{
	    if( *strText == ']' )
	    {
		*( category_text + category_text_length ) = '|';
		category_text_length += 1;
		checker_category_text = false;
	    }
	    else
	    {
		if( *strText != ',' )
		{
		    int len = mblen(&strText[0], MB_CUR_MAX);
		    if( len > 1 )
		    {
			if( category_text_length + len >= TEXT_MAX_LENGTH )
			{
			    category_text_length = TEXT_MAX_LENGTH;
			}
			else
			{
			    int unicode_num = 0;
			    for( unicode_num = 0; unicode_num < len; unicode_num ++)
			    {
				*(category_text + category_text_length ) =*strText;
				strText += 1;
				category_text_length += 1;
			    }
			    strText -= 1;
			}
		    }
		    else
		    {
			*(category_text+category_text_length) = *strText;
			category_text_length += 1;
		    }
		}
	    }   
	}
    }
    if( category_text_length == 0 )
    {
	*category_text = '|' ;
    }
}

int parser_brace(char *strText)
{
    bool check_brace = true;
    int brace_num = 0;
    int jump_text_num = 0;
    while( check_brace )
    {
	if( *(strText + jump_text_num) == '{' )
	{
	    brace_num+=1;
	}
	else if( *(strText + jump_text_num) =='}' )
	{
	    brace_num-=1;
	}

	if( brace_num <= 0 )
	{
	    check_brace = false;
	    return jump_text_num+1;
	}
	else
	{
	    jump_text_num +=1;
	}
    }
}

int parser_html_math(char *strText)
{
    int html_math_total_num = 0;
    while( *(strText+html_math_total_num+0) == '<' &&
	   *(strText+html_math_total_num+1) == 'm' &&
	   *(strText+html_math_total_num+2) == 'a' &&
	   *(strText+html_math_total_num+3) == 't' &&
	   *(strText+html_math_total_num+4) == 'h' )
    {
	bool checker_html_math = true;
	html_math_total_num += 5;
	while( checker_html_math )
	{
	    if( *(strText+html_math_total_num+0) == '<' &&
		*(strText+html_math_total_num+1) == '/' &&
		*(strText+html_math_total_num+2) == 'm' &&
		*(strText+html_math_total_num+3) == 'a' &&
		*(strText+html_math_total_num+4) == 't' &&
		*(strText+html_math_total_num+5) == 'h' &&
		*(strText+html_math_total_num+6) == '>')
	    {
		html_math_total_num += 7;
		checker_html_math = false;
	    }
	    else
	    {
		html_math_total_num +=1;
	    }
	}


    }
    return html_math_total_num;
}

int parser_html_font(char *strText)
{
    int html_font_total_num = 0;
    while( *(strText + html_font_total_num + 0 ) == '<' &&
	   *(strText + html_font_total_num + 1 ) == 'f' &&
	   *(strText + html_font_total_num + 2 ) == 'o' &&
	   *(strText + html_font_total_num + 3 ) == 'n' &&
	   *(strText + html_font_total_num + 4 ) == 't' &&
	   *(strText + html_font_total_num + 5 ) != ' ' )
    {
	bool checker_html_font = true;
	html_font_total_num += 5;
	while( checker_html_font )
	{
	    if( *(strText + html_font_total_num + 0 ) == '<' &&
		*(strText + html_font_total_num + 1 ) == '/' &&
		*(strText + html_font_total_num + 2 ) == 'f' &&
		*(strText + html_font_total_num + 3 ) == 'o' &&
		*(strText + html_font_total_num + 4 ) == 'n' &&
		*(strText + html_font_total_num + 5 ) == 't' &&
		*(strText + html_font_total_num + 6 ) == '>' )
	    {
		html_font_total_num += 7;
		checker_html_font = false;
	    }
	    else
	    {
		html_font_total_num += 1;
	    }
	}
    }
    return html_font_total_num;
}

int parser_html_ref(char *strText)
{
    int html_ref_total_num = 0;
    while( *(strText + html_ref_total_num + 0 ) == '<' &&
	   *(strText + html_ref_total_num + 1 ) == 'r' &&
	   *(strText + html_ref_total_num + 2 ) == 'e' &&
	   *(strText + html_ref_total_num + 3 ) == 'f' )
    {
	bool checker_html_ref = true;
	html_ref_total_num += 4;
	while( checker_html_ref )
	{
	    if (*(strText + html_ref_total_num ) == '>' )
	    {
		html_ref_total_num += 1;
		checker_html_ref = false;
	    }
	    else
	    {
		html_ref_total_num += 1;
	    }
	}

    }
    return html_ref_total_num;
}
int parser_html_category(char* strText)
{
    int html_category_num = 0;
    while( *(strText + html_category_num + 0  ) == '[' &&
	   *(strText + html_category_num + 1  ) == '[' &&
	   *(strText + html_category_num + 2  ) == 'C' &&
	   *(strText + html_category_num + 3  ) == 'a' &&
	   *(strText + html_category_num + 4  ) == 't' &&
	   *(strText + html_category_num + 5  ) == 'e' &&
	   *(strText + html_category_num + 6  ) == 'g' &&
	   *(strText + html_category_num + 7  ) == 'o' &&
	   *(strText + html_category_num + 8  ) == 'r' &&
	   *(strText + html_category_num + 9  ) == 'y' &&
	   *(strText + html_category_num + 10 ) == ':' )
    {
	bool checker_html_category = true;
	html_category_num += 11;
	while( checker_html_category )
	{
	    if( *(strText + html_category_num + 0 ) == ']' &&
		*(strText + html_category_num + 1 ) == ']' )
	    {
		html_category_num += 2;
		checker_html_category = false;
	    }
	    else
	    {
		html_category_num += 1;
	    }
    	}
    }
    return html_category_num;
}

int parser_html_div(char * strText)
{
    int html_div_total_num = 0;
    while( (*(strText + html_div_total_num + 0 ) == '<' &&
	    *(strText + html_div_total_num + 1 ) == 'd' &&
	    *(strText + html_div_total_num + 2 ) == 'i' &&
	    *(strText + html_div_total_num + 3 ) == 'v') ||
	   (*(strText + html_div_total_num + 0 ) == '<' &&
	    *(strText + html_div_total_num + 1 ) == 'D' &&
	    *(strText + html_div_total_num + 2 ) == 'I' &&
	    *(strText + html_div_total_num + 3 ) == 'V') ||
	   (*(strText + html_div_total_num + 0 ) == '<' &&
	    *(strText + html_div_total_num + 1 ) == '/' &&
	    *(strText + html_div_total_num + 2 ) == 'd' &&
	    *(strText + html_div_total_num + 3 ) == 'i' &&
	    *(strText + html_div_total_num + 4 ) == 'v'))
    {
	bool checker_html_div = true;
	html_div_total_num += 4;
	while ( checker_html_div )
	{
	    if( *(strText + html_div_total_num ) == '>' )
	    {
		html_div_total_num += 1;
		checker_html_div = false;
	    }
	    else
	    {
		html_div_total_num += 1;
	    }
	}
    }
    return html_div_total_num;
}


void string_cutoff(char *strText)
{
//    setlocale( LC_CTYPE, "" );
//    setlocale( LC_ALL, "en_US.utf8" );
    int modify_text_num = 0;
    modify_text = (char*)malloc(sizeof(char)*TEXT_MAX_LENGTH);
    for(modify_text_num = 0; modify_text_num <= TEXT_MAX_LENGTH; modify_text_num++)
    {
	*(modify_text+modify_text_num)='\0';
    }
    modify_text_num = 0;
    int checker_i = 0;
    int checker_j = 0;
    int jump_num = 0;
    bool word_checker = false;
    for(; *strText != '\0'; strText++)
    {
	word_checker = false;
	if(modify_text_num >= TEXT_MAX_LENGTH)
	{
	    
	    cout<<"length : "<<modify_text_num<<endl;
	    return;
	}

	//check to brace marker
	while( *strText =='{' )
	{
	    jump_num = parser_brace(strText);
	    strText+=jump_num;
	    word_checker = true;
	}

	//check to html_math func
	jump_num = parser_html_math(strText);
	strText+=jump_num;
	if( jump_num > 0 )
	{
	    word_checker = true;
	}
	
	//check to html_font func
        jump_num = parser_html_font(strText);
        strText+=jump_num;
	if( jump_num > 0 )
	{
	    word_checker = true;
	}

	//check to html_ref func
        jump_num = parser_html_ref(strText);
        strText+=jump_num;
	if( jump_num > 0 )
	{
	    word_checker = true;
	}

	jump_num = parser_html_category(strText);
	strText+=jump_num;
	if( jump_num > 0 )
	{
	    word_checker = true;
	}

	jump_num = parser_html_div(strText);
	strText += jump_num;
	if( jump_num > 0 )
	{
	    word_checker = true;
	}
	
	for( checker_i = 0; checker_i < word_separation_max ; checker_i ++)
	{
	    bool checked_word_separation = true;
	    if( *((*(word_separation_text+checker_i))) == *strText)
	    {
		for(checker_j = 0; checker_j < *(word_separation_length+checker_i); checker_j ++)
		{
		    if( *((*(word_separation_text+checker_i))+checker_j) != *(strText+checker_j))
		    {
			checked_word_separation = false;
			checker_j = TEXT_MAX_LENGTH;
		    }
		}
	    }
	    else
		checked_word_separation = false;
	    
	    
	    if( checked_word_separation )
	    {
		strText+=*(word_separation_length+checker_i);
		checker_i = word_separation_max;
		word_checker = true;
	    }
	}

	if( word_checker )
	{
	    strText =-1;
	    continue;
	}

	if(*strText == '*' ||
	   *strText == '=' ||
	   *strText == '\'' ||
	   *strText == '{' ||
	   *strText == '}' ||
	   *strText == '\n' ||
	   *strText == ',' ||
	   *strText == ';' ||
	   *strText == '\x5C')
	{
	    continue;
	}
	else
	{
	    int len = mblen(&strText[0],MB_CUR_MAX);
	    if( len > 1 ) 
	    {
		if( modify_text_num + len >= TEXT_MAX_LENGTH )
		{
		    cout<<"end of length : "<<len<<" : "<<modify_text_num<<endl;
		    modify_text_num += len;
		}
		else
		{
		    int unicode_num = 0;

		    for( unicode_num = 0; unicode_num < len; unicode_num ++)
		    {
			*(modify_text + modify_text_num) = *strText;
			strText+=1;
			modify_text_num +=1;
		    }
		    strText-=1;
		}
	    }
	    else
	    {
		*(modify_text+modify_text_num) = *strText;
		modify_text_num+=1;
	    }
	}
    }

    cout<<"length : "<<modify_text_num<<endl;
}


void string_parser(char *strText)
{
    if( NULL == strText )
    {
	printf("null\n");
	return;
    }
    delete_enter(strText);
    parsing_category(strText);
    string_cutoff(strText);
}

void separation_word()
{
    *(word_separation_text+word_separation_max++) = (char*)"<ul>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<li>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<em>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</ul>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</li>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</em>\0";
    *(word_separation_text+word_separation_max++) = (char*)"[[\0";
    *(word_separation_text+word_separation_max++) = (char*)"]]\0";
    *(word_separation_text+word_separation_max++) = (char*)"<small>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</small>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<br>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<br >\0";
    *(word_separation_text+word_separation_max++) = (char*)"<br />\0";
    *(word_separation_text+word_separation_max++) = (char*)"<BR>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<br/>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</br/>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<br/ >\0";
    *(word_separation_text+word_separation_max++) = (char*)"<ref>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</ref>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<code>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</code>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<table alignright>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</table>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<tt>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</tt>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<sup>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</sup>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<nowiki>\0";
    *(word_separation_text+word_separation_max++) = (char*)"</nowiki>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<HR>\0";
    *(word_separation_text+word_separation_max++) = (char*)"<!--\0";
    *(word_separation_text+word_separation_max++) = (char*)"-->\0";
    *(word_separation_text+word_separation_max++) = (char*)"<REF>";
    *(word_separation_text+word_separation_max++) = (char*)"</REF>";
    *(word_separation_text+word_separation_max++) = (char*)"<center>";
    *(word_separation_text+word_separation_max++) = (char*)"</center>";
    *(word_separation_text+word_separation_max++) = (char*)"&amp;";
    *(word_separation_text+word_separation_max++) = (char*)"</div>";
    *(word_separation_text+word_separation_max++) = (char*)"</DIV>";
    int i,j = 0;

    word_separation_length = (int*)malloc(sizeof(int)*word_separation_max);
    for( i  =0;i<word_separation_max; i++)
    {
	*(word_separation_length+i) = strlen(*(word_separation_text+i));
   }
}


int main()
{
    setlocale( LC_CTYPE, "" );
//    setlocale( LC_ALL, "" );
//    setlocale( LC_ALL, "ja_JP.utf8" );
    separation_word();
    using namespace tinyxml2;
    XMLDocument xml;

    xml.LoadFile("wiki_split_aa.xml");
//    xml.LoadFile("enwiki01.xml");
//    xml.LoadFile("jawiki01.xml");
/*
    XMLElement *elem = xml.FirstChildElement("page")->FirstChildElement("title");
    std::cout<<elem->GetText()<<std::endl;
*/

    XMLElement *pageElm = xml.FirstChildElement("page");
    XMLElement *textElm = NULL;
    const XMLAttribute *textAttr = NULL;
    //FILE *fp;
    //fp = fopen("test.csv","wn");
    ofstream fp;
    fp.open("mini_test.csv");
//    fp.open("jp_test.csv");
//    fp.open("en_test.csv");
    const char *bom = "\xEF\xBB\xBF";
    fp << bom;
    
    //fp << "\xEF\xBB\xBF"; 
    int i = 0;
    int number =1;
    while( pageElm != NULL )
    {
	if( pageElm->FirstChildElement("redirect") == NULL )
	{
//	   cout<<"Title : "<<pageElm->FirstChildElement("title")->GetText()<<endl;
	   char *strTitle = (char*)(pageElm->FirstChildElement("title")->GetText());
	   int n_title = 0;
	   for( ; *strTitle != '\0'; strTitle++ )
	   {
	       if( *strTitle == ',' ||
		   *strTitle == ';' )
		   *strTitle ='|';
	       n_title +=1;
	   }
	   strTitle-=n_title;

	   textElm = pageElm->FirstChildElement("revision")->FirstChildElement("text");
//	   cout<<textElm->GetText()<<endl;
//	   textAttr = textElm->FirstAttribute();
//	   strText = (char*)textAttr->Value();

	   char *strText = (char*)(textElm->GetText());
	   if( strText == NULL )
	   {
//	       cout<<"NULL"<<endl;
	       pageElm = (XMLElement *)(pageElm->NextSibling());
	       continue;
	   }
	   string_parser(strText);
	   cout<<"Num : "<<number << endl;
	   cout<<"Title : "<<strTitle<<endl;
	   cout<<"Category : "<<category_text<<endl;
	   cout<<modify_text<<endl<<endl<<endl<<endl;
	   number+=1;
	  // fprintf(fp,"%s, %s",strTitle, modify_text);
	   fp<<strTitle<<","<<category_text<<","<<modify_text<<endl;
	   i+=1;
//	   cout<<strlen(modify_text)<<endl;
	   free(modify_text);
	   free(category_text);
	}
	pageElm = (XMLElement *)(pageElm->NextSibling());
    }
    //fclose(fp);
    fp.close();
    return 1;

}
