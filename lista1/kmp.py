import sys


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




def Compute_Prefix_Function(pattern):
	m=len(pattern)
	pi=[0]*m
	pi[0]=-1
	k=-1
	for q in range (1,m):
		while ((k>-1) and (pattern[k+1]!=pattern[q])):
			k=pi[k]
		if(pattern[k+1]==pattern[q]):
			k=k+1
		pi[q]=k
	#print(pi)
	return pi





def main():

	string=""
	with open(sys.argv[2],"r") as f:
		string=f.read()
	pattern=sys.argv[1]
	KMP_Matcher(string,pattern)


if __name__=="__main__":
	main()