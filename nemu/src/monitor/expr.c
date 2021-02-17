#include "nemu.h"
#include "cpu/reg.h"
#include "cpu/cpu.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

uint32_t look_up_symtab(char *sym, bool *success);
enum
{
	NOTYPE = 256,
	EQ,
	NEQ,
	NUM,
	REG,
	DEREF,
	HEX,
	LS,
	RS,
	LE,
	GE,
	AND,
	OR,
	NEG,
	SYMB

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"\\-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"%",'%'},
	{"0[xX][0-9a-fA-F]+", HEX},
	{"[0-9]+", NUM},
	{"==", EQ},
	{"!=", NEQ},
	{"\\$e[abcd]x",REG},
	{"\\$e[sd]i",REG},
	{"\\$e[bis]p",REG},
	{"[a-zA-z]+[0-9a-zA-Z_]*",SYMB},
	{"\\(",'('},
	{"\\)",')'},
	{"<<",LS},
	{">>",RS},
	{"<=",LE},
	{">=",GE},
	{"<",'<'},
	{">",'>'},
	{"&&",AND},
	{"\\|\\|",OR},
	{"&",'&'},
	{"\\|",'|'},
	{"!",'!'},
	{"\\^",'^'},
	{"~",'~'},

};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				case NOTYPE: break;
				case REG:
				case HEX:
				case NUM:
				case SYMB:
				    for (int j = 0; j < substr_len; j++){
				        tokens[nr_token].str[j] = *(substr_start + j);
				    }
				        tokens[nr_token].str[substr_len] = '\0';
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

uint32_t check_parentheses(int p, int q){
    int j = 0;
    for (int i = p; i < q; i++){
        if (tokens[i].type == '(') j++;
        if (tokens[i].type == ')') j--;
        if (j <= 0) return 0;
    }
    if (tokens[q].type == ')') j--;
    return (j == 0);
}

static struct opr_pri{
    int opr;
    int pri;
}list[]={
    {OR, 12},
    {AND, 11},
    {'|', 10},
    {'^', 9},
    {'&', 8},
    {EQ, 7},
    {NEQ, 7},
    {'<', 6},
    {'>', 6},
    {LE, 6},
    {GE, 6},
    {LS, 5},
    {RS, 5},
    {'+', 4},
    {'-', 4},
    {'*', 3},
    {'/', 3},
    {'%', 3},
    {'!', 2},
    {'~', 2},
    {DEREF, 2},
    {NEG, 2},
};

int list_num = sizeof(list) / sizeof(list[0]);
uint32_t eval(int p, int q, bool *success) {
    if(p > q) {
        /* Bad expression */
        *success = false;
        return 0;
    }
    else if(p == q) { 
        /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
         uint32_t temp;
         switch (tokens[p].type){
             case HEX:
                sscanf(tokens[p].str, "%x", &temp);
                return temp;
            case NUM:
                sscanf(tokens[p].str, "%d", &temp);
                return temp;
            case REG:
                if (!memcmp(tokens[p].str, "$eax", 4)) return cpu.eax;
                if (!memcmp(tokens[p].str, "$ebx", 4)) return cpu.ebx;
                if (!memcmp(tokens[p].str, "$ecx", 4)) return cpu.ecx;
                if (!memcmp(tokens[p].str, "$edx", 4)) return cpu.edx;
                if (!memcmp(tokens[p].str, "$esi", 4)) return cpu.esi;
                if (!memcmp(tokens[p].str, "$edi", 4)) return cpu.edi;
                if (!memcmp(tokens[p].str, "$esp", 4)) return cpu.esp;
                if (!memcmp(tokens[p].str, "$eip", 4)) return cpu.eip;
                if (!memcmp(tokens[p].str, "$ebp", 4)) return cpu.ebp;
            case SYMB: return look_up_symtab(tokens[p].str, success);
            default: assert(0); return 0;
         }
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */
        return eval(p + 1, q - 1, success); 
    }
    else {
        //op = the position of dominant operator in the token expression;
        int op = -1;
        int op_pri = 0;
        for (int i = p; i < q; i++){
            if (tokens[i].type == '('){
                int temp = 1;
                while (temp != 0){
                    i++;
                    if (tokens[i].type == '(') temp++;
                    if (tokens[i].type == ')') temp--;
                }
            }
            else {
                int j;
                for (j = 0; j < list_num; j++) if (list[j].opr == tokens[i].type ) break;
                if (j != list_num && list[j].pri >= op_pri){
                    op_pri = list[j].pri;
                    op = i;
                }
            }
            
        }
        
        int op_type = tokens[op].type;
        uint32_t val1 = 0;
        if (op_type != DEREF && op_type != NEG && op_type != '!' && op_type != '~')
            val1 = eval(p, op - 1, success);
        uint32_t val2 = eval(op + 1, q, success);
        switch(op_type) {
            case '+': return val1 + val2; break;
            case '-': return val1 - val2; break;
            case '*': return val1 * val2; break;
            case '/': return val1 / val2; break;
            case '%': return val1 % val2; break;
            case OR : return val1 || val2; break;
            case AND: return val1 && val2; break;
            case '|': return val1 | val2; break;
            case '^': return val1 ^ val2; break;
            case '&': return val1 & val2; break;
            case EQ : return val1 == val2; break;
            case NEQ: return val1 != val2; break;
            case '<': return val1 < val2; break;
            case '>': return val1 > val2; break;
            case LE : return val1 <= val2; break;
            case GE : return val1 >= val2; break;
            case LS : return val1 << val2; break;
            case RS : return val1 >> val2; break;
            case '!': return !val2; break;
            case '~': return ~val2; break;
            case NEG: return -val2; break;
            case DEREF: return vaddr_read(*(get_mem_addr() + val2), 0, 4); break;
            default: assert(0);
        }
    }
    return 0;
}

bool judge(int i){
    if (tokens[i].type == NOTYPE) {
        while (tokens[i].type == NOTYPE && i >= 0) i--;
    }
    
    return (tokens[i].type != HEX) && (tokens[i].type != NUM) && (tokens[i].type != REG) && (tokens[i].type != ')') && (tokens[i].type != SYMB);
}
uint32_t expr(char *e, bool *success)
{
	*success = true;
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
    
    /* TODO: Implement code to evaluate the expression. */  
    for(int i = 0; i < nr_token; i ++) {
        if(tokens[i].type == '*' && (i == 0 || judge(i - 1)) ) {
        tokens[i].type = DEREF;
        }
        else if (tokens[i].type == '-' && ((i == 0) || judge(i - 1))){
            tokens[i].type = NEG;
        }
    }
    return eval(0, nr_token - 1, success);
    
	//printf("\nPlease implement expr at expr.c\n");
	//fflush(stdout);
	//assert(0);

	return 0;
}
