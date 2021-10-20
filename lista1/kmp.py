#Radosław Wojtczak nr indeksu: 254607
#JFTT Lista 1, Algorytm Knutha-Morrisa-Prata
#źródło implementacji algorytmów (pdf książki):
# https://ressources.unisciel.fr/algoprog/s00aaroot/aa00module1/res/%5BCormen-AL2011%5DIntroduction_To_Algorithms-A3.pdf

import sys

#Główna funkcja mająca za zadanie sprawdzenie, czy istnieje pattern w stringu

def KMP_Matcher(string,pattern):
	n=len(string)
	m=len(pattern)
	matches=[]
	pi=Compute_Prefix_Function(pattern)
	q=0
	for i in range(n):
		while ((q>0) and (pattern[q]!=string[i])):
			q=pi[q-1]+1
		if (pattern[q]==string[i]):
			q=q+1
		if q==m:
			matches.append(i-m+1)
			q=pi[q-1]+1
	print("{} - {}".format(pattern,matches))


#Generowanie "tablicy powrotów"

def Compute_Prefix_Function(pattern):
	m=len(pattern)
	pi=[0]*m
	pi[0]=-1
	k=-1
	while_counter=0
	for q in range (1,m):
		print("{} - {}".format(k,q))
		#pomijam pierwsze sprawdzenie, niech będzie to operacja wykonująca się w czasie stałym (jak operacja przypisywania)
		while ((k>-1) and (pattern[k+1]!=pattern[q])):
			print("Siema")
			while_counter=while_counter+1;
			k=pi[k]
		if(pattern[k+1]==pattern[q]):
			k=k+1
		pi[q]=k
	#print(pi)
	print(while_counter)
	return pi





def main():

	#Wczytywanie danych i uruchamianie odpowiednich funkcji
	string=""
	with open(sys.argv[2],"r") as f:
		string=f.read()
	pattern=sys.argv[1]
	KMP_Matcher(string,pattern)

#Rozruch
if __name__=="__main__":
	main()