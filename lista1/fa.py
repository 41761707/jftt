#Radosław Wojtczak nr indeksu:254607
#JFTT Lista 1, Deterministyczny Automat Skończony
#źródło implementacji algorytmów (pdf książki):
#https://ressources.unisciel.fr/algoprog/s00aaroot/aa00module1/res/%5BCormen-AL2011%5DIntroduction_To_Algorithms-A3.pdf



import sys

def Finite_Automation_Matcher(string,pattern,delta):
	n=len(string)
	m=len(pattern)
	q=0
	matches=[]
	for i in range(n):
		q=delta[q,string[i]]
		if q==m:
			matches.append(i-m+1)
	print("{} - {}".format(pattern,matches))


#Funkcja obliczajaca funkcje przejscia (oznaczana na wykladzie przez małą deltę)

def Compute_Transition_Function(pattern,alphabet,delta):
	m=len(pattern)
	for q in range(m+1):
		for a in alphabet:
			k=min(m,q+1)
			temp=pattern[:q]+a
			while (not (temp.endswith(pattern[:k]))):
				k=k-1
				if(k==0):
					break
			delta[q,a]=k




def main():
	#Wczytywanie danych, tworzenie slownika oraz uruchamianie funkcji
	delta={}
	string=""
	with open(sys.argv[2],"r") as f:
		string=f.read()
	pattern=sys.argv[1]
	listpattern=list(pattern)
	alphabet=set(string)
	Compute_Transition_Function(pattern,alphabet,delta)
	Finite_Automation_Matcher(string,pattern,delta)

#rozruch
if __name__=='__main__':
	main()