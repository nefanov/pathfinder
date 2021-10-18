"""
constructs NFG from a set of words (extensively naive)
"""
import pprint

def GetKey(val, d):
   for key, value in d.items():
      if val == value:
         return key

   return "key doesn't exist"

def construct(W=['hi', 'how', 'are', 'you'], n=2):
	S_storage=[] 
	num_cnt = 0
	grammar_table = {}
	l_name = ""
	for i, s in enumerate(W):
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
		# compose rules Nj --> NiNk
		while (len(chunks)>1):
			chunks = [list(chunks[i:i + n])
			 for i in range(0, len(chunks) - (len(chunks) % n) + 1, n)]

			if (len(chunks[-1])== 0):
				chunks = chunks[:-1]
			new_chunks = []

			for right in chunks:
				if right in grammar_table.values(): # X ::== right
					l_name = GetKey(right, grammar_table) # lname <- X
				else:
					if (len(right)==2):
						num_cnt += 1
						l_name = "N"+str(num_cnt)
						grammar_table.update({l_name:right})
					else:
						l_name = right[0]
				
					
				new_chunks.append(l_name)

			chunks = new_chunks

		l_name = "IS"+str(i)
		S_storage.append([l_name])
		grammar_table.update({l_name:chunks[0]})


		# remove chain rules
		k = GetKey(grammar_table[chunks[0]], grammar_table)
		grammar_table.update({l_name: grammar_table[k]})
		del grammar_table[k]

	d = [(key,v) for key,v in grammar_table.items()]
	for key,v in d:
		if key.startswith('IS'):
			grammar_table.update({key[1:]:v})
	return grammar_table


if __name__ == "__main__":
	gt = construct()
	print("=====grammar======")
	pprint.pprint(gt)
