#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char* argv[]) {
	int n = 0;//n - показывает, строка внутри функции или снаружи
	vector <string> l;//хранит названия функций
	vector <int> begin;//хранит все номера
	vector <int> num;//хранит, какие номера соответствуют какой функции в begin
	vector <int> phi;//хранит номер тонкого блока, где есть phi
	vector <int> phinum;//хранит, какие phi в какой функции
	int i = 0, b = 0, e = 0, pr = 0, k = 0, q = 0, c = 0, r = 0, sw = 0, u = 0, j = 0;
	//b, e - начало и конец подстрок; i - номера блоков/операций вообще до разбиения на тонкие блоки; q - номера блоков/операций внутри каждой функции; 
	//pr - проверяет, соответствовует ли строка началу блока до разбиения на тонкие блоки; k - показывает, первый ли это встретившийся номер в функции;
	//r - количество функций; sw - есть ли switch в строке; u - количество phi-функций в каждой функции; 
	//j - просто нужна для хранения того, что ей присвоили, особенного смысла не имеет; c - номер строки.
	std::string s, s0, s1, st, str, pred, predpred;
	s = "clang -S -emit-llvm ";
	s += argv[1];
	system(s.c_str());
	s = argv[1];
	s1 = s.substr(s.rfind(" ") + 1, s.rfind("/") - s.rfind(" "));
	s0 = s.substr(s.rfind("/") + 1, s.rfind(".") - s.rfind("/") - 1);
	s = s0;
	s += ".ll";
	ifstream file (s.c_str());
	s = s0;
	s += "0.ll";
	ofstream f (s.c_str());
	num.push_back(0);
	phinum.push_back(0);
	while (getline(file, st)) {//1ый проход по .ll файлу, разбиение на тонкие блоки, присвоение каждому блоку и каждой операции нового номера, также учет частных случаев (switch, phi и т д)
		if (st.find("define") != -1) {
			b = st.find_first_of("@");
			e = st.find_first_of("( ", b);
			str = st.substr(b + 1, e - b - 1);
			l.push_back(str);
			n = 1;
			b = 0;
			e = 0;
			r += 1;
			f << st << endl;
		}
		else if (n == 1) {
			c += 1;
		 	if (st != "}") {
		 		if (st.find("preds") != -1) {
		 			pr = c;
		 			b = st.find_first_of(":");
		 			str = st.substr(0, b);
		 			begin.push_back(q);
		 			st.replace(0, str.size(), std::to_string(q));
		 			q += 1;
		 			i += 1;
		 		}
		 		if ((pr == 0) && (q != 0) && (st != "") && (sw == 0)) {
					f << q << ":" << endl;
					q += 1;
				}
				if (st.find("switch") != -1) sw = 1;
				b = st.find_first_of("%");
				e = st.find_first_of(",) ", b);
				if (b != -1) {
					if (e != -1) str = st.substr(b + 1, e - b - 1);
					else str = st.substr(b + 1);
					if (str.find("struct") == -1) {
						k += 1;
						if (b == 2) {
							if (k == 1) q = atoi(str.c_str());
							begin.push_back(q);
							st.replace(b + 1, str.size(), std::to_string(q));
							q += 1;
							i += 1;
						}
					}
					while ((st.find_first_of("%", e + 1) != -1) && (e != -1)) {
						b = st.find_first_of("%", e + 1);
						e = st.find_first_of(",) ", b);
						if (b != -1) {
							str = st.substr(b + 1, e - b - 1);
							if (str.find("struct") == -1) {
								k += 1;
							}
						}
					}
				}
				f << st << endl;
				if ((st.find("preds") == -1) && (st != "") && (st.find("br") == -1) && (st.find("ret") == -1) 
				&& (st.find("unreachable") == -1) && (sw == 0)) f << "  br label %" << q << endl;
				if ((st.find("]") != -1) && (st.find("phi") == -1)) sw = 0;
				if ((c - pr == 1) && (pr != 0)) pr = 0;
				if (st.find("phi") != -1) {
					phi.push_back(q - 1);
					u += 1;
				}
			} else {
				phinum.push_back(u);
				num.push_back(i);
				n = 0;
				k = 0;
				f << st << endl;
				q = 0;
				pr = 0;
				u = 0;
			}
		}
		else if (st.find("attributes") == -1) f << st << endl;
	}
	f.close();
	file.close();
	s = s0;
	s += "0.ll";
	ifstream f1 (s.c_str());
	s = s0;
	s += "1.ll";
	ofstream f2 (s.c_str());
	r = 0;
	vector <string> phi1(phi.size() + 1);
	vector <string> phi2(phi.size() + 1);
	u = 0;
	while (getline(f1, st)) {//2ой проход по .ll файлу, замена всех номеров на новые номера, также работа с phi
		if (st.find("define") != -1) { 
			n = 1;
			r += 1;
		}
		else if ((n == 1) && ((st.find("br label") == -1) || ((st.find("br label") != -1) && (pred.find(":") != -1)))) {
			if (st != "}") {
				if (st.find("preds") != -1) {
					b = st.find_first_of(";");
					st = st.substr(0, b);
				}
				b = st.find_first_of("%");
				e = st.find_first_of(",) ]", b);
				if (b != -1) {
					if (e != -1) str = st.substr(b + 1, e - b - 1);
					else str = st.substr(b + 1);
					if (str.find("struct") == -1) {
						k += 1;
						if ((b == 2) && (st.find("phi") != -1)) {
							for (int j = phinum[r - 1]; j < phinum[r]; j++) {
								if (str == to_string(phi[j])) u = j;
							}
						}
						if ((b > 2) && (str.find("struct") == -1)) {
							if (atoi(str.c_str()) < begin[num[r - 1]]) j = atoi(str.c_str());
							else j = begin[num[r - 1] + atoi(str.c_str()) - begin[num[r - 1]]];
							if ((st.find("phi") != -1) && (st.find("]", b) == e + 1)) {
								if (phi1[u] != "!") {
									st.replace(b + 1, (phi1[u]).size(), phi1[u]);
									phi1[u] = "!";
								} else st.replace(b + 1, (phi2[u]).size(), phi2[u]);
							} else st.replace(b + 1, str.size(), to_string(j));
						}
					}
					while ((st.find_first_of("%", e + 1) != -1) && (e != -1)) {
						b = st.find_first_of("%", e + 1);
						e = st.find_first_of(",) ]", b);
						if (b != -1) {
							str = st.substr(b + 1, e - b - 1);
							if (str.find("struct") == -1) {
								if (atoi(str.c_str()) < begin[num[r - 1]]) j = atoi(str.c_str());
								else j = begin[num[r - 1] + atoi(str.c_str()) - begin[num[r - 1]]];
								if ((st.find("phi") != -1) && (st.find("]", b) == e + 1)) {
									if (phi1[u] != "!") {
										st.replace(b + 1, (phi1[u]).size(), phi1[u]);
										phi1[u] = "!";
									} else st.replace(b + 1, (phi2[u]).size(), phi2[u]);
								} else st.replace(b + 1, str.size(), to_string(j));
							}
						}
					}
				}
			} else {
				n = 0;
			}
		}
		f2 << st << endl;
		for (int j = phinum[r - 1]; j < phinum[r]; j++) {
			s = "%";
			s += to_string(phi[j] - 1);
			if ((st.find("br") != -1) && (st.find(s) != -1)) {
				if (pred.find(":") != -1) {
					b = pred.find_first_of(":");
					str = pred.substr(0, b);
				} else if (predpred.find(":") != -1) {
					b = predpred.find_first_of(":");
					str = predpred.substr(0, b);
				}
				if (phi1[j] == "") phi1[j] = str;
				else phi2[j] = str;
			}
		}
		predpred = pred;
		pred = st;
	}
	f1.close();
	f2.close();
	//конец работы с .ll файлами
	//начало работы с dot файлами: их создание, сшивка в 1 файл, также удаление .ll файлов
	s = "opt -dot-cfg ";
	s += s0;
	s += "1.ll";
	system(s.c_str());
	s = s0;
	s += ".ll";
	remove(s.c_str());
	s = s0;
	s += "0.ll";
	remove(s.c_str());
	s = s0;
	s += "1.ll";
	remove(s.c_str());
	file.open(".main.dot");
	f.open("main");
	while (getline(file, st)) {
		if ((st.find("}") == -1) || (st.find("}\"") != -1)) 
			f << st << endl;
	}
	file.close();
	f.close();
	file.open("main");
	f.open(".main.dot");
	while (getline(file, st)) f << st << endl;//добавляется содержимое dot файла функции main
	file.close();
	f.close();
	fstream ODOT (".main.dot", std::ofstream::app);
	vector <string> index;
	for (int i = 0; i < l.size(); i++) {//сшивка - добавление содержимого dot файлов других функций (не main)
		s = ".";
		s += l[i];
		s += ".dot";
		if (s.find(".main.dot") == -1) {
			file.open(s.c_str());
			ODOT << "\tsubgraph cluster_" + l[i] + " {" << endl;
			ODOT << "\t\tlabel = \"CFG for '" + l[i] + "' function\";" << endl;
			ODOT << "\t\tgraph[style=filled, bgcolor=white];" << endl;
			n = 0;
			while (getline(file, st)) {
				if ((st.find("\"CFG ") == -1) && (st.find("Node") != -1)) n += 1;
				if (n == 1) {
					index.push_back(st.substr(st.find("Node"), st.find_first_of(": ") - 1));
				}	
				if ((st.find("label=\"CFG") == -1) && (st.find("digraph") == -1) && 
				((st.find("}\"") != -1) || (st.find("}") == -1))) 
					ODOT << "\t" + st << endl;
			}
			ODOT << "\t}" << endl;
			file.close();
			remove(s.c_str());
		} else index.push_back("");
	}
	ODOT << "}" << endl;
	ODOT.close();
	file.open(".main.dot");
	f.open("main");
	i = 0;
	while (getline(file, st)) {//отрисовка стрелочек между функциями
		f << st << endl;
		f2 << st << endl;
		if (st.find("@") != -1) {
			b = st.find_first_of("@");
			e = st.find_first_of("( ", b);
			str = st.substr(b + 1, e - b - 1);
			while ((str != l[i]) && (i < index.size())) i += 1;
			if (i != index.size()) 
				f << st.substr(0, st.find_first_of(": ")) + " -> " + index[i] + " [style = dotted];" << endl;
			i = 0;
		}	
	}
	file.close();
	f.close();
	file.open("main");
	f.open((s1 + ".main" + s0 + ".dot").c_str());//создается окончательный dot файл
	while (getline(file, st)) f << st << endl;
	file.close();
	f.close();
	
	system(("dot -Tps " + s1 + ".main" + s0 + ".dot -o " + s1 + "outfile" + s0 + ".ps").c_str());//визуализация графа
	remove((s1 + ".main.dot").c_str());
	remove(".main.dot");
	remove("main");
	remove("transl");
}