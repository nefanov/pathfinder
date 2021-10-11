"""
constructs NFG from a set of words (extensively naive)
"""
import pprint

def GetKey(val, d):
   for key, value in d.items():
      if val == value:
         return key
      return "key doesn't exist"

def construct(W=["aaaa"], n=2):

	
	num_cnt = 0
	grammar_table = {}
	l_name = ""
	for s in W:
	# compose rules Nj --> a
		chunks = []
		for right in s:
			if right in grammar_table.values():
				l_name = GetKey(right, grammar_table)
			else:
				num_cnt += 1
				l_name = "N"+str(num_cnt)
				grammar_table.update({l_name:right})
			
			chunks.append(l_name)
	print("chunks 1:", chunks)
	# compose rules Nj --> NiNk
	while (len(chunks)>1):
		chunks = [list(chunks[i:i + n]) for i in range(0, len(chunks) - (len(chunks) % n) + 1, n)]
		
		print("len:", len(chunks))
		if (len(chunks[-1])== 0):
			chunks = chunks[:-1]
		print("postproc chunks:", chunks)
		new_chunks = []
		for right in chunks:
			print("right:",right)
			if right in grammar_table.values():
				l_name = GetKey(right, grammar_table)
			else:
				num_cnt += 1
				l_name = "N"+str(num_cnt)
				grammar_table.update({l_name:right})
			new_chunks.append(l_name)

		chunks = new_chunks
		print("Chunks:", chunks)
		pprint.pprint(grammar_table)
		input()

	l_name = "S"
	grammar_table.update({l_name:chunks[0]})
	pprint.pprint(grammar_table)
	

	# remove chain rules

if __name__ == "__main__":
	construct()