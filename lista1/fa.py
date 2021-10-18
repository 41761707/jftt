import sys

def Finite_Automation_Matcher(string,pattern,delta):
	n=len(string)
	m=len(pattern)
	q=0
	matches=[]
	for i in range(n):
		q=delta[q,string[i]]
		#print("{} - {} - {}".format(i,q,string[i]))
		if q==m:
			#print("Pattern occurs with shift {}".format(i-m+1))
			matches.append(i-m+1)
	print("{} - {}".format(pattern,matches))


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
			#print("q: {}, k: {}, a: {}, pattern[:q]+a: {}, pattern[:k] {}".format(q,k,a,temp,pattern[:k]))
			delta[q,a]=k




def main():
	delta={}
	string=""
	with open(sys.argv[2],"r") as f:
		string=f.read()
	#print(string)
	pattern=sys.argv[1]
	listpattern=list(pattern)
	#print(listpattern)
	alphabet=set(string)
	#print(alphabet)
	#print("Tekst: {}, Wzorzec: {}".format(string,pattern))
	Compute_Transition_Function(pattern,alphabet,delta)
	#print(delta)
	Finite_Automation_Matcher(string,pattern,delta)

if __name__=='__main__':
	main()