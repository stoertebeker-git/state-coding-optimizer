// k7scan1.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#include "node.h"
#include "helper.h"
#include "binary.h"
#include "table.h"
#include "stdafx.h"
#include <ctime>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <bitset>
#include <list>

using namespace std;

#define	Getc(s)			getc(s)
#define	Ungetc(c)		{ungetc(c,IP_Input); ugetflag=1;}

/*
 *	Lexical analyzer states.
 */
enum lexstate {
	L_START, L_INT, L_IDENT, L_STRING, L_STRING2,
	L_COMMENT, L_TEXT_COMMENT, L_LINE_COMMENT, L_END_TEXT_COMMENT, L_DEFSTATE, L_DEFIN, L_DEFOUT, E_INNAMES, E_OUTNAMES, E_INVALUE, E_OUTVALUE, E_NODES};
const int STRING1 = 3;
const int IDENTIFIER = 4;
const int INTEGER1 = 5;
const int DEFSTATE = 6;
const int DEFIN = 7;
const int DEFOUT = 8;
const int EDGE = 9;
const int TOKENSTART = 300;

class CParser
{
public:

	string yytext;								//input buffer
	struct tyylval{								//value return
		string s;								//structure
		int i;
	}yylval;
	struct newEdge {
		string from = "";						//start node
		string to = "";							//end node
		vector<char> values;					// contains values of the selected in- or outputs, necessary if not all are used
		list<string> names;						// contains names of the selected in- or outputs, necessary if not all are used or oder varies
		list<vector<bool>> in;					//contains the read condition, times 2 elements for each don't care
												//x is split up in 1 and 0
	} edge;
	FILE *IP_Input;								//Input File
	FILE *IP_Error;								//Error Output
	FILE *IP_List;								//List Output
	int  IP_LineNumber;							//Line counter
	int ugetflag;								//checks ungets
	int prflag;									//controls printing
	map<string,int> IP_Token_table;				//Tokendefinitions
	map<int,string> IP_revToken_table;			//reverse Tokendefinitions

	vector<Node*> nodes;
	map<string, int> nodeMap; //maps node name to index in vector nodes, so it is not necessary to iterate through whole vector nodes to find a certain node. maps have faster look up operations
	map<string, int > inputNames; //names of input bits	
	map<string, int> outputNames; //names of output bits
	vector<Binary*> conditions; //contains all possible conditions	
	vector<Binary*> outputs; //contains all possible outputs
	string f = ""; //helper

	lexstate s;

	int CParser::yylex();						//lexial analyser
	void CParser::yyerror(char *ers);			//error reporter
	int CParser::IP_MatchToken(string &tok);	//checks the token
	void CParser::InitParse(FILE *inp,FILE *err,FILE *lst);
	int	CParser::yyparse();						//parser
	void CParser::pr_tokentable();				//test output for tokens
	void CParser::IP_init_token_table();		//loads the tokens
	void CParser::Load_tokenentry(string str,int index);//load one token
	void CParser::PushString(char c);			//Used for string assembly
	CParser(){IP_LineNumber = 1;ugetflag=0;prflag=0;};	//Constructor
	int CParser::lexHelper(vector<char> split, vector<char> end, lexstate e, int c);
};
//------------------------------------------------------------------------

// Adds a character to the string value
void CParser::PushString(char c)
{
	yylval.s += c;
}
//------------------------------------------------------------------------
void CParser::Load_tokenentry(string str,int index)
{
	IP_Token_table[str]=index;
	IP_revToken_table[index]=str;
}
void CParser::IP_init_token_table()
{
	Load_tokenentry("STRING1",3);
	Load_tokenentry("IDENTIFIER",4);
	Load_tokenentry("INTEGER1",5);
	Load_tokenentry("DEFSTATE", 6);
	Load_tokenentry("DEFIN", 7);
	Load_tokenentry("DEFOUT", 8);
	Load_tokenentry("[", 9);
	Load_tokenentry("=(", 10);
	Load_tokenentry(">[", 11);
	Load_tokenentry(":(", 12);
	Load_tokenentry(")(", 13);

	int ii=TOKENSTART;
	Load_tokenentry("AND",ii++);
	Load_tokenentry("OR",ii++);
	Load_tokenentry("Begin",ii++);
	Load_tokenentry("End",ii++);

}
//------------------------------------------------------------------------

void CParser::pr_tokentable()
{
	
	typedef map<string,int>::const_iterator CI;
	const char* buf;

	printf( "Symbol Table ---------------------------------------------\n");

	for(CI p=IP_Token_table.begin(); p!=IP_Token_table.end(); ++p){
		buf = p->first.c_str();
		printf(" key:%s", buf);
		printf(" val:%d\n", p->second);;
	}
}
//------------------------------------------------------------------------

int	CParser::yyparse()
{
	int tok;
	if(prflag)fprintf(IP_List,"%5d ",(int)IP_LineNumber);
	/*
	*	Go parse things!
	*/
	while ((tok=yylex())!=0){
		printf("%d ",tok);
		if(tok==STRING1)
			printf("%s %s ",IP_revToken_table[tok].c_str(),yylval.s.c_str());
		else
			if(tok==INTEGER1)
				printf("%s %d ",IP_revToken_table[tok].c_str(),yylval.i);
			else
				if(tok==IDENTIFIER)
					printf("%s %s ",IP_revToken_table[tok].c_str(),yylval.s.c_str());
				else
					if(tok>=TOKENSTART)
						printf("%s ",IP_revToken_table[tok].c_str());
					else
						printf("%c ",tok);
		if(!prflag)printf("\n");
	}
	return 0;

}
//------------------------------------------------------------------------

/*
 *	Parse Initfile:
 *
 *	  This builds the context tree and then calls the real parser.
 *	It is passed two file streams, the first is where the input comes
 *	from; the second is where error messages get printed.
 */
void CParser::InitParse(FILE *inp,FILE *err,FILE *lst)

{

	/*
	*	Set up the file state to something useful.
	*/
	IP_Input = inp;
	IP_Error = err;
	IP_List  = lst;

	IP_LineNumber = 1;
	ugetflag=0;
	/*
	*	Define both the enabled token and keyword strings.
	*/
	IP_init_token_table();
}
//------------------------------------------------------------------------

/*
 *	yyerror:
 *
 *	  Standard error reporter, it prints out the passed string
 *	preceeded by the current filename and line number.
 */
void CParser::yyerror(char *ers)

{
  fprintf(IP_Error,"line %d: %s\n",IP_LineNumber,ers);
}
//------------------------------------------------------------------------

int CParser::IP_MatchToken(string &tok)
{
	int retval;
	if(	IP_Token_table.find(tok) != IP_Token_table.end()){
		retval = (IP_Token_table[tok]);
	}else{
		retval = (0);
	}
	return retval;
} 

//------------------------------------------------------------------------

/*
 *	yylex:
 *
 */
int CParser::yylex()
{
	//locals of yylex()
	int c;

	vector<char> split;
	vector<char> end;

	/*
	*	Keep on sucking up characters until we find something which
	*	explicitly forces us out of this function.
	*/
	for (s = L_START; 1;){
		c = Getc(IP_Input);
		yytext = yytext + (char)c;
		if(!ugetflag) { 
			if(c != EOF)if(prflag)fprintf(IP_List,"%c",c);
		}else ugetflag = 0;
		switch (s) {
			//Starting state, look for something resembling a token.
		case L_START:
			if (isdigit(c)) {
				s = L_INT;
			}
			else if (isalpha(c)) {
				s = L_IDENT;
				Ungetc(c); //helps to use chars as identifier, otherwise at first entry in L_STRING yytext.size() = 2
				yytext = yytext.substr(0, yytext.size() - 1);
			}

			else if (c == '=' || c == ':' || c == '>' || c == ')' ) {
				s = L_IDENT;
				yytext = c;
			}
			else if (c == '\\' || c == '[' || c == '(') {
				s = L_IDENT;
				Ungetc(c); //helps to use chars as identifier, otherwise at first entry in L_STRING yytext.size() = 2
				yytext = yytext.substr(0, yytext.size() - 1);

			}
			else if (isspace(c)) {
				if (c == '\n') {
					IP_LineNumber += 1;
					if (prflag)
						fprintf(IP_List, "%5d ", (int)IP_LineNumber);
				}
				yytext = yytext.substr(0, yytext.size() - 1); //space is not supposed to reset yytext
			}
			else if (c == '/') {
				yytext = "";
				s = L_COMMENT;
			}
			else if (c == '"') {
				s = L_STRING;
			}
			else if (c == EOF) {
				return ('\0');
			}
			else {
				return (c);
			}
			break;

		case L_COMMENT:
			if (c == '/')
				s = L_LINE_COMMENT;
			else	if (c == '*')
				s = L_TEXT_COMMENT;
			else {
				Ungetc(c);
				return('/');	/* its the division operator not a comment */
			}
			break;
		case L_LINE_COMMENT:
			if (c == '\n') {
				s = L_START;
				Ungetc(c);
			}
			yytext = "";
			break;
		case L_TEXT_COMMENT:
			if (c == '\n') {
				IP_LineNumber += 1;
			}
			else if (c == '*')
				s = L_END_TEXT_COMMENT;
			yytext = "";
			break;
		case L_END_TEXT_COMMENT:
			if (c == '/') {
				s = L_START;
			}
			else {
				s = L_TEXT_COMMENT;
			}
			yytext = "";
			break;

			/*
			 *	Suck up the integer digits.
			 */
		case L_INT:
			if (isdigit(c)) {
				break;
			}
			else {
				Ungetc(c);
				yylval.s = yytext.substr(0, yytext.size() - 1);
				yylval.i = atoi(yylval.s.c_str());
				return (INTEGER1);
			}
			break;

			/*
			 *	Grab an identifier, see if the current context enables
			 *	it with a specific token value.
			 */

		case L_IDENT:
			int a;

				while (yytext.find(' ') != std::string::npos) {
					yytext.replace(yytext.find(' '), 1, "");
				}

			
			if (a = IP_MatchToken(yytext)) {
				//cout << "entered token comparison. Got token " << IP_MatchToken(yytext) << endl;
				//TODO replace with map if possible
				if (a == 6) {
					s = L_DEFSTATE;
				}
				else if (a == 7) {
					s = L_DEFIN;
				}
				else if (a == 8) {
					s = L_DEFOUT;
				}
				else if (a == 9) {
					s = E_INNAMES;
				}
				else if (a == 10) {
					s = E_INVALUE;
					int j = inputNames.size();
					while (j > 0) {
						edge.values.push_back('x');
						j--;
					}
				}
				else if (a == 11) {
					s = E_OUTNAMES;
				}
				else if (a == 12) {
					s = E_OUTVALUE;
					int j = outputNames.size();
					while (j > 0) {
						edge.values.push_back('0'); //no don't care values for output otherwise a certian input has multiple output options 
						j--;
					}
				}
				else if (a == 13) {
					s = E_NODES;
					
				}
				//cout << "switched state to " << s << endl;
				yytext = " ";
				break;
			}
			if (isalpha(c) || isdigit(c) || c == '=' || c == ':' || c == '>' || c == ')' || c == ' ') {
				break;
			} else {
			Ungetc(c);
			yytext = yytext.substr(0, yytext.size() - 1);
			yylval.s = yytext;
			return (IDENTIFIER);
			}

		case L_DEFSTATE:
			//cout << "entered DEFSTATE" << endl;
			split = { ',', ';'};
			end = {';' };
			lexHelper(split, end, L_DEFSTATE,c);
			break;


		case L_DEFIN:
			//cout << "entered DEFIN" << endl;
			split = { ',', ';' };
			end = { ';' };
			lexHelper(split, end, L_DEFIN, c);
			break;


		case L_DEFOUT:
			//cout << "entered DEFOUT" << endl;
			split = { ',', ';' };
			end = { ';' };
			lexHelper(split, end, L_DEFOUT, c);
			break;


		case E_INNAMES:
			//cout << "entered E_INNAMES" << endl;
			split = { ',', ']' };
			end = { ']' };
			lexHelper(split, end, E_INNAMES, c);
			break;


		case E_INVALUE:
			//cout << "entered E_INVALUE" << endl;
			split = { ',', ')' };
			end = { ')' };
			lexHelper(split, end, E_INVALUE, c);
			break;

		case E_OUTNAMES:
			//cout << "entered E_OUTNAMES" << endl;
			split = { ',', ']' };
			end = { ']' };
			lexHelper(split, end, E_OUTNAMES, c);
			break;


		case E_OUTVALUE:
			//cout << "entered E_OUTVALUE" << endl;
			split = { ',', ')' };
			end = { ')' };
			lexHelper(split, end, E_OUTVALUE, c);
			break;


		case E_NODES:
			//cout << "entered E_NODES" << endl;
			end = { ')' };
			lexHelper(end, end, E_NODES, c);
			break;

					
		   /*
		   *	Suck up string characters but once resolved they should
		   *	be deposited in the string bucket because they can be
		   *	arbitrarily long.
		   */
			case L_STRING2:
				s = L_STRING;
				if(c == '"'){
					PushString((char)c);
				}else{
					if(c == '\\'){
						PushString((char)c);
					}else{
						PushString((char)'\\');
						PushString((char)c);
					}
				}
			break;
			case L_STRING:
				if (c == '\n')
				  IP_LineNumber += 1;
				else if (c == '\r')
						;
					 else	if (c == '"' || c == EOF){
								return (STRING1);
							} else	if(c=='\\'){
										s = L_STRING2;
										//PushString((char)c);
									}else
										PushString((char)c);
			break;
			default: printf("***Fatal Error*** Wrong case label in yylex\n");
		}
	}
}

int CParser::lexHelper(vector<char> split, vector<char> end, lexstate e, int c) {
	int t;
	bool b = (find(split.begin(), split.end(), c) != split.end() || find(end.begin(), end.end(), c) != end.end()); // checks if c is seperator or end sign
	if (b) {
		yytext = yytext.substr(0, yytext.size() - 1);

		while (yytext.find(' ') != std::string::npos) {
			yytext.replace(yytext.find(' '), 1, "");
		}
		yylval.s = yytext; // last read char is not part of state name
	
		switch (e) {		
		
		case L_DEFSTATE:
			nodeMap.insert(make_pair(yytext, nodes.size()));
			nodes.push_back(new Node(yytext));
			cout << "added node " << yytext << endl;
			break;

		case L_DEFIN:
			inputNames.insert(make_pair(yytext, inputNames.size()));
			cout << "added input " << yytext << endl;
			if (find(end.begin(), end.end(), c) != end.end()) {
				for (int i = 0; i < pow(2, inputNames.size()); i++) {
					conditions.push_back(new Binary(i));
				}
			}
			break;

		case L_DEFOUT:
			outputNames.insert(make_pair(yytext, outputNames.size()));
			cout << "added output " << yytext << endl;
			if (find(end.begin(), end.end(), c) != end.end()) {
				for (int i = 0; i < pow(2, outputNames.size()); i++) {
					outputs.push_back(new Binary(i));
				}
			}
			break;
		
		case E_INNAMES:
			edge.names.push_back(yytext);
			cout << "added condition input name " << yytext << " of edge" << endl;

			break;

		case E_INVALUE:
			f = *edge.names.begin();
			if (inputNames.count(f) != 0) {
				t = inputNames.find(f)->second;
				edge.names.pop_front();
				if (yytext == "0") {
					edge.values[t] = '0';
				}
				else if (yytext == "1") {
					edge.values[t] = '1';
				}

				cout << "added condition input value " << yytext << " of edge" << endl;

				if (find(end.begin(), end.end(), c) != end.end()) {
					dontCareBits(edge.values, edge.in);
				}
			}
			else {
				cout << "Not able to retrieve position of " << *edge.names.begin() << endl;
			}
			
			break;

		case E_OUTNAMES:
			edge.names.push_back(yytext);
			cout << "added output name " << yytext << " of edge" << endl;
			break;

		case E_OUTVALUE:
			f = *edge.names.begin();
			if (outputNames.count(f) != 0) {
				t = outputNames.find(f)->second;
				edge.names.pop_front();
				if (yytext == "0") {
					edge.values[t] = '0';
				}
				else if (yytext == "1") {
					edge.values[t] = '1';
				}

				cout << "added condition output value " << yytext << " of edge" << endl;

				if (find(end.begin(), end.end(), c) != end.end()) {

					list<vector<bool>>::iterator iter = edge.in.begin();
					
					while (iter != edge.in.end()) {
						nodes.at(nodeMap.find(edge.from)->second)->setOutputAt(outputs[boolVecToInt(charVecToBoolVec(edge.values))], conditions[boolVecToInt(*iter)]);
						cout << "added new output for node " + edge.from << endl;
						iter++;
					}
					
				}
			}
			else {
				cout << "Not able to retrieve position of " << *edge.names.begin() << endl;
			}

			break;

		case E_NODES:
			if (edge.from.empty()) {
				edge.from = yytext;
				cout << "added start node " << yytext << " of edge" << endl;
				edge.values.clear();
				edge.names.clear(); //should be empty
			}
			else {
				edge.to = yytext;
				cout << "added end node " << yytext << " of edge" << endl;

				//TODO add to node element
				if (find(end.begin(), end.end(), c) != end.end()) {

					list<vector<bool>>::iterator iter = edge.in.begin();

					while (iter != edge.in.end()) {
						nodes[nodeMap.find(edge.from)->second]->newConnection(nodes[nodeMap.find(edge.to)->second], conditions[boolVecToInt(*iter)]);
						cout << "added new connection from node " + edge.from  + " to " + edge.to << endl;
						iter++;
					}

				}

				//clean up
				edge.from = "";
				edge.to = "";
				edge.in.clear();
				edge.names.clear();
				edge.values.clear();

				cout << "cleaned up for next edge" << endl;
			}

			break;
		}
		yytext = " ";
		if ((find(end.begin(), end.end(), c) != end.end())) {
			s = L_START;
			if (c == ')' && e != E_NODES) {
				yytext = ")";
			}
			return(c);
		}
		return 0;
	}
	if (isalpha(c) || isdigit(c) || c == ' ') {
		return 0;
	}
	else { //read some different char, not wanted in name
		Ungetc(c);
		yytext = yytext.substr(0, yytext.size() - 1);
		yylval.s = yytext;
		cout << "exited " << e << endl;
		return (DEFIN);
	}
}
//------------------------------------------------------------------------
int main(int argc, char** argv) {
	FILE *inf;
	char fistr[100];
	printf("Enter filename:\n");
	gets_s(fistr);
	inf = fopen(fistr, "r");
	if (inf == NULL) {
		printf("Cannot open input file %s\n", fistr);
		return 0;
	}
	CParser obj;
	obj.InitParse(inf, stderr, stdout);
	//	obj.pr_tokentable();
	obj.yyparse();

	// new stuff
	setNumNodes(obj.nodes.size());


	returnPriorityOne(obj.nodes, obj.conditions);
	returnPriorityTwo(obj.nodes, obj.conditions);
	returnPriorityThree(obj.nodes, obj.conditions);

	writeFile(obj.nodes, obj.conditions);
	generateOutput(obj.nodes);

	Table* table = new Table(pow(2, bitSize(obj.nodes.size())));
	table->assignCodes(obj.nodes);
	for (Node* &n : obj.nodes) {
		if (n->getNodeCode() != NULL)
			cout << n->getName() << ": " << printVec(n->getNodeCode()->returnAsBoolVec(), false) << endl;
	}

	cout << "PRIO 1 NODES NEIGHBOURED = " << table->getSuccess(0) << endl
		<< "PRIO 2 NODES NEIGHBOURED = " << table->getSuccess(1) << endl
		<< "PRIO 3 NODES NEIGHBOURED = " << table->getSuccess(2) << endl;

	printSortedMLFile(table, obj.conditions);
	printUnsortedMLFile(obj.nodes, obj.conditions);

	// Clean up
	delete table;
	for (auto &n : obj.nodes)
		delete n;

	for (auto &n : obj.conditions)
		delete n;

	return 0;
}
