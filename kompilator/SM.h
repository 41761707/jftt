
enum code_ops { HALTx, STOREx, JMP_FALSEx, GOTOx,DATAx, LD_INTx, LD_VARx,GET_I, PUT_I,LTx, EQx, GTx, ADDx, SUBx, MULTx, DIVx, PWRx };
char *op_name[] = {"HALT", "store", "jmp_false", "goto","data", "ld_int", "ld_var","GET", "PUT","lt", "eq", "gt", "ADD", "SUB", "mult", "div", "pwr" };
struct instruction
{
	enum code_ops op;
	int arg;
};
struct instruction code[999];
int stack[999];
int pc = 0;
struct instruction ir;
int ar = 0;
int top = 0;
void fetch_execute_cycle()
{ 
	do 
	{ 

		ir = code[pc++];
		/*printf( "PC = %3d IR.arg = %8d IR.op %3d AR = %3d Top = %3d,%8d\n",
		pc, ir.arg, ir.op,ar, top, stack[top]);*/
		/* Execute */
		switch (ir.op) 
		{
			case HALTx : 
				printf( "halt\n" ); 
			break;
			case GET_I : 
				printf( "Input: " );
				scanf( "%ld", &stack[ar+ir.arg] ); 
			break;
			case PUT_I : 
				printf( "Output: %d\n", stack[top--] ); 
			break;
			case STOREx : 
				stack[ir.arg] = stack[top--]; 
			break;
			case JMP_FALSEx : 
			if ( stack[top--] == 0 )
			{
				pc = ir.arg;
			}
			break;
			case GOTOx : 
				pc = ir.arg; 
			break;
			case DATAx : 
				top = top + ir.arg; 
			break;
			case LD_INTx : 
				stack[++top] = ir.arg; 
			break;
			case LD_VARx: 
				stack[++top] = stack[ar+ir.arg]; 
			break;
			case LTx : 
				if ( stack[top-1] < stack[top] )
				{
					stack[--top] = 1;
				}
				else 
				{
					stack[--top] = 0;
				}
			break;
			case EQx : 
				if ( stack[top-1] == stack[top] )
				{
					stack[--top] = 1;
				}
				else 
				{
					stack[--top] = 0;
				}
			break;
			case GTx : 
				if ( stack[top-1] > stack[top] )
				{
					stack[--top] = 1;
				}
				else 
				{
					stack[--top] = 0;
				}
			break;
			case ADDx : 
				stack[top-1] = stack[top-1] + stack[top];
				top--;
			break;
			case SUBx : 
				stack[top-1] = stack[top-1] - stack[top];
				top--;
			break;
			case MULTx : 
				stack[top-1] = stack[top-1] * stack[top];
				top--;
			break;
			case DIVx : 
				stack[top-1] = stack[top-1] / stack[top];
				top--;
			break;
			case PWRx : 
				stack[top-1] = stack[top-1] * stack[top];
				top--;
			break;
				default : printf( "%sInternal Error: Memory Dump\n" );
			break;
		}
	}
	while (ir.op != HALTx);
}