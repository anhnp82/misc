int solution(std::vector<int> &A, int M)
{
  unsigned int click(0);
  
  for(int i=0;i< A.size();++i)
  {
	std::set<int> B;
	for(int j=0;j< A.size();++j)
	{
	  if(abs(A[i]-A[j])%M == 0)
	  {
		B.insert(j);
	  }
	}

	if(click < B.size())
	click = B.size();
  }
  
  return click;
}