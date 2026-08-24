#include "stdio.h"

int main(){
	int n1=0, n2=1;
	int res;
	int opc;
	printf("Cuantos numeros quieres");
	scanf("%d",&opc);
	printf("%d", n1);
	for(int i=0; i<opc-1; i++){
	res= n1 + n2;
	n1=n2;
	n2=res;
	printf(" %d  ",res);
	}
}
