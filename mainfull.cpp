#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

class BT_Node;

class Node{
public:
	int data = 0;
	Node * nextField = NULL;
	BT_Node * self = NULL;

	Node(int val){
		data = val;
		return;
	}
};

class List_Node{
public:
	Node * cur = NULL;
	List_Node * nxt = NULL;
	List_Node * bfr = NULL;

	List_Node(Node * tmp){
		cur = tmp;
		return;
	}
};

class Node_List{
public:
	List_Node * first = NULL;
	List_Node * last = NULL;
	int sz = 0;

	void add(Node * tmp){
		List_Node * tmpnode = new List_Node(tmp);
		sz ++;

		if(first == NULL){
			first = tmpnode;
			last = tmpnode;
			return;
		}

		(*last).nxt = tmpnode;
		(*tmpnode).bfr = last;
		last = tmpnode;
		return;
	}

	void rmv(Node * tmp){
		sz --;

		List_Node * cur = first;
		while(cur != NULL && (*cur).cur != tmp) cur = (*cur).nxt;
		if(cur == NULL){
			sz ++;
			return;
		}
		if(sz == 0){
			first = NULL;
			last = NULL;
			return;
		}

		if((*cur).bfr != NULL) (*(*cur).bfr).nxt = (*cur).nxt;
		else first = (*first).nxt;
		if((*cur).nxt != NULL) (*(*cur).nxt).bfr = (*cur).bfr;
		else last = (*last).bfr;
		return;
	}
};

class B_Tree;
class BT_Block;

class BT_Node{
public:
	BT_Node(Node * tmp){
		self = new Node_List();
		(*self).add(tmp);
		val = (*tmp).data;
		(*tmp).self = this;
		return;
	}
private:
	int val;
	Node_List * self = NULL;
	BT_Node * nxt = NULL;
	BT_Node * bfr = NULL;
	BT_Block * rch = NULL;
	BT_Block * lch = NULL;
	friend class BT_Block;
	friend class B_Tree;
};

class BT_Block{
public:
	BT_Block(BT_Node * tmp){
		first = tmp;
		sz = 1;
		return;
	}
	void Add(BT_Node * tmp){
		sz ++;
		BT_Node * curn = first;
		if((*tmp).rch != NULL) (*(*tmp).rch).par = this;
		if((*tmp).lch != NULL) (*(*tmp).lch).par = this;

		if(first == NULL){
			first = tmp;
			(*tmp).bfr = NULL, (*tmp).nxt = NULL;
			return;
		}

		if((*curn).val > (*tmp).val){
			first = tmp;
			(*tmp).nxt = curn, (*tmp).bfr = NULL;
			(*curn).bfr = tmp;
			(*curn).lch = (*tmp).rch;
			return;
		}

		while((*curn).val < (*tmp).val && (*curn).nxt != NULL) curn = (*curn).nxt;

		if((*curn).val < (*tmp).val){
			(*curn).nxt = tmp;
			(*tmp).bfr = curn, (*tmp).nxt = NULL;
			(*curn).rch = (*tmp).lch;
			return;
		}

		if((*curn).val == (*tmp).val){
			sz --;
			List_Node * cur = (*(*tmp).self).first;
			for(int i = 0; i < (*(*tmp).self).sz; i++){
				(*(*curn).self).add((*cur).cur);
				cur = (*cur).nxt;
			}
			return;
		}

		(*tmp).bfr = (*curn).bfr;
		(*curn).bfr = tmp;
		(*(*tmp).bfr).nxt = tmp;
		(*tmp).nxt = curn;
		(*(*tmp).bfr).rch = (*tmp).lch;
		(*(*tmp).nxt).lch = (*tmp).rch;
		return;
	}
	bool leaf(){
		if((*first).lch == NULL) return true;
		return false;
	}
	void Rmv(BT_Node * tmp){
		sz --;
		if(first == tmp) first = (*first).nxt;
		if((*tmp).bfr != NULL) (*(*tmp).bfr).nxt = (*tmp).nxt;
		if((*tmp).nxt != NULL) (*(*tmp).nxt).bfr = (*tmp).bfr;
		return;
	}
	BT_Node * Rmostchild(){
		BT_Node * curn = first;
		if(curn == NULL) return curn;
		while((*curn).nxt != NULL) curn = (*curn).nxt;
		return curn;
	}
private:
	int sz = 0;
	BT_Node * first = NULL;
	BT_Block * par = NULL;
	friend class B_Tree;
};

class B_Tree{
public:

	BT_Block * Find(int val, BT_Block * cur){
		if(cur == NULL) return NULL;
		BT_Node * curn = (*cur).first;
		if(curn == NULL) return NULL;
		BT_Block * nxt = NULL;
		while((*curn).val < val && (*curn).nxt != NULL) curn = (*curn).nxt;

		if((*curn).val == val) return cur;
		if((*curn).val < val) nxt = (*curn).rch;
		else nxt = (*curn).lch;
		if(nxt == NULL) return cur;
		return Find(val, nxt);
	}

	Node_List * FindEQ(int val){
		BT_Block * cur = Find(val, root);
		BT_Node * curn = (*cur).first;
		while(curn != NULL && (*curn).val != val) curn = (*curn).nxt;

		Node_List * ans = new Node_List();
		if(curn == NULL) return ans;
		List_Node * curtmp = (*(*curn).self).first;
		for(int i = 0; i < (*(*curn).self).sz; i++, curtmp = (*curtmp).nxt){
			(*ans).add((*curtmp).cur);
		}
		return ans;
	}

	void FindBG(int val, Node_List * FTN, bool rt, BT_Block * cur = NULL){
		if(rt) cur = root;
		if(cur == NULL) return;
		BT_Node * curn = (*cur).first;
		while(curn != NULL){
			if((*curn).val > val){
				List_Node * CUR = (*(*curn).self).first;
				for(int i = 0; i < (*(*curn).self).sz; i++){
					(*FTN).add((*CUR).cur);
					CUR = (*CUR).nxt;
				}
				FindBG(val, FTN, 0, (*curn).lch);
			}
			if((*curn).nxt == NULL) FindBG(val, FTN, 0, (*curn).rch);
			curn = (*curn).nxt;
		}
		return;
	}

	void FindSM(int val, Node_List * FTN, bool rt, BT_Block * cur = NULL){
		if(rt) cur = root;
		if(cur == NULL) return;
		BT_Node * curn = (*cur).first;
		if(curn != NULL) FindSM(val, FTN, 0, (*curn).lch);
		while(curn != NULL){
			if((*curn).val < val){
				List_Node * CUR = (*(*curn).self).first;
				for(int i = 0; i < (*(*curn).self).sz; i++){
					(*FTN).add((*CUR).cur);
					CUR = (*CUR).nxt;
				}
				FindSM(val, FTN, 0, (*curn).rch);
			}
			else break;
			curn = (*curn).nxt;
		}
		return;
	}

	bool FindN(Node * tmpnode){
		BT_Block * cur = Find((*tmpnode).data, root);
		if(cur == NULL) return false;
		BT_Node * curn = (*cur).first;
		while(curn != NULL && (*curn).val != (*tmpnode).data) curn = (*curn).nxt;
		if(curn == NULL) return false;
		List_Node * curtmp = (*(*curn).self).first;
		for(int i = 0; i < (*(*curn).self).sz; i++, curtmp = (*curtmp).nxt){
			if((*curtmp).cur == tmpnode) return true;
		}
		return false;
	}

	void Add(Node * tmpnode){
		int val = (*tmpnode).data;
		BT_Block * cur = Find(val, root);
		BT_Node * tmp = new BT_Node(tmpnode);
		if(cur == NULL){
			root = new BT_Block(tmp);
			return;
		}
		(*cur).Add(tmp);
		if((*cur).sz > T - 1) RA(cur);
		return;
	}

	void Rmv(Node * tmpnode){
		int val = (*tmpnode).data;
		BT_Block * cur = Find(val, root);
		if(cur == NULL) return;

		BT_Node * curn = (*cur).first;
		while(curn != NULL && (*curn).val != val) curn = (*curn).nxt;
		if(curn == NULL) return;
		if((*(*curn).self).sz > 1){
			(*(*curn).self).rmv(tmpnode);
			return;
		}

		if((*cur).leaf()){
			(*cur).Rmv(curn);
			if((*cur).sz < ((T + 1) / 2) - 1) RAR(cur);
		}
		else{
			BT_Block * new_cur = (*curn).lch;
			BT_Block * nxt = (*(*new_cur).Rmostchild()).rch;
			while(nxt != NULL){
				new_cur = nxt;
				nxt = (*(*new_cur).Rmostchild()).rch;
			}
			BT_Node * new_curn = (*new_cur).Rmostchild();
			int tmpval = (*curn).val;
			Node_List * tmpself = (*curn).self;
			(*curn).val = (*new_curn).val, (*curn).self = (*new_curn).self;
			(*new_curn).val = tmpval, (*new_curn).self = tmpself;
			(*new_cur).Rmv(new_curn);
			if((*new_cur).sz < ((T + 1) / 2) - 1) RAR(new_cur);
		}
		return;
	}

	void chp(BT_Block * cur = NULL, int cnt = 0){
		if(cur == NULL) cur = root;
		BT_Node * curn = (*cur).first;
		while(curn != NULL){
			cout << "val : ";
			List_Node * CUR = (*(*curn).self).first;
			while(CUR != NULL){
				cout << (*(*CUR).cur).data << " ";
				CUR = (*CUR).nxt;
			}
			cout << " cnt : " << cnt;
			if((*cur).par != NULL) cout << " par : " << (*(*(*cur).par).first).val << endl;
			else cout << " par : " << -1 << endl;
			curn = (*curn).nxt;
		}
		curn = (*cur).first;
		while(curn != NULL){
			if((*curn).lch != NULL) chp((*curn).lch, ++cnt);
			if((*curn).nxt == NULL && (*curn).rch != NULL) chp((*curn).rch, ++cnt);
			curn = (*curn).nxt;
		}
		return;
	}

private:
	const int T = 3;
	BT_Block * root = NULL;

	void RA(BT_Block * cur){
		BT_Node * curn = (*cur).first;

		BT_Block * L = new BT_Block(curn);
		for(int i = 1; i <= T/2; i++, curn = (*curn).nxt){
			if((*curn).lch != NULL) (*(*curn).lch).par = L;
			if((*curn).rch != NULL) (*(*curn).rch).par = L;
		}
		(*(*curn).bfr).nxt = NULL;
		(*L).sz = T / 2, (*L).par = (*cur).par;

		BT_Node * UP = curn;

		curn = (*curn).nxt;
		BT_Block * R = new BT_Block(curn);

		(*curn).bfr = NULL;
		for(int i = T/2 + 2; i <= T; i++, curn = (*curn).nxt){
			if((*curn).lch != NULL) (*(*curn).lch).par = R;
			if((*curn).rch != NULL) (*(*curn).rch).par = R;
		}
		(*R).sz = T - (T/2) - 1, (*R).par = (*cur).par;

		(*UP).nxt = NULL, (*UP).bfr = NULL;
		(*UP).lch = L, (*UP).rch = R;

		if((*cur).par == NULL){
			root = new BT_Block(UP);
			(*L).par = root, (*R).par = root;
			return;
		}

		BT_Block * ucur = (*cur).par;
		(*ucur).Add(UP);
		if((*ucur).sz > T - 1) RA(ucur);
		return;
	}

	void RAR(BT_Block * cur, BT_Block * ls = NULL){
		if((*cur).par == NULL){
			if((*cur).sz == 0){
				if((*ls).sz == 0) root = NULL;
				else{
					root = ls;
					(*ls).par = NULL;
				}
			}
			return;
		}

		BT_Block * lbro = NULL;
		BT_Block * rbro = NULL;
		BT_Node * par_curn = (*(*cur).par).first, * par_lbro = NULL, * par_rbro = NULL;
		while(par_curn != NULL){
			if((*par_curn).rch == cur){
				lbro = (*par_curn).lch;
				par_lbro = par_curn;
			}
			if((*par_curn).lch == cur){
				rbro = (*par_curn).rch;
				par_rbro = par_curn;
			}
			par_curn = (*par_curn).nxt;
		}

		if(lbro != NULL && (*lbro).sz >= ((T + 1) / 2)){
			BT_Node * lbro_curn = (*lbro).Rmostchild();
			(*lbro).Rmv(lbro_curn);
			(*lbro_curn).lch = (*lbro_curn).rch;
			if((*cur).first != NULL) (*lbro_curn).rch = (*(*cur).first).lch;
			else (*lbro_curn).rch = ls;
			(*cur).Add(lbro_curn);

			int tmpval = (*lbro_curn).val;
			Node_List * tmplist = (*lbro_curn).self;
			(*lbro_curn).val = (*par_lbro).val, (*lbro_curn).self = (*par_lbro).self;
			(*par_lbro).val = tmpval, (*par_lbro).self = tmplist;
			return;
		}
		if(rbro != NULL && (*rbro).sz >= ((T + 1) / 2)){
			BT_Node * rbro_curn = (*rbro).first;
			(*rbro).Rmv(rbro_curn);
			(*rbro_curn).rch = (*rbro_curn).lch;
			if((*cur).first != NULL) (*rbro_curn).lch = (*(*cur).Rmostchild()).rch;
			else (*rbro_curn).lch = ls;
			(*cur).Add(rbro_curn);

			int tmpval = (*rbro_curn).val;
			Node_List * tmplist = (*rbro_curn).self;
			(*rbro_curn).val = (*par_rbro).val, (*rbro_curn).self = (*par_rbro).self;
			(*par_rbro).val = tmpval, (*par_rbro).self = tmplist;
			return;
		}
		BT_Block * nls = NULL;
		if(lbro != NULL){
			(*(*cur).par).Rmv(par_lbro);
			if((*par_lbro).nxt != NULL) (*(*par_lbro).nxt).lch = lbro;
			BT_Node * lst = (*lbro).Rmostchild();
			(*par_lbro).lch = (*lst).rch;
			if((*cur).first != NULL) (*par_lbro).rch = (*(*cur).first).lch;
			else{
				(*par_lbro).rch = ls;
				if(ls != NULL) (*ls).par = lbro;
			}
			BT_Node * nw = par_lbro;
			(*lbro).sz ++, (*lst).nxt = nw, (*nw).bfr = lst, (*nw).nxt = NULL;
			lst = nw, nw = (*cur).first;
			while(nw != NULL){
				(*lbro).sz ++, (*lst).nxt = nw, (*nw).bfr = lst;
				if((*nw).lch != NULL) (*(*nw).lch).par = lbro;
				if((*nw).rch != NULL) (*(*nw).rch).par = lbro;
				lst = nw, nw = (*nw).nxt;
			}
			nls = lbro;
		}
		else{
			(*(*cur).par).Rmv(par_rbro);
			if((*par_rbro).bfr != NULL) (*(*par_rbro).bfr).rch = rbro;
			BT_Node * lst = (*rbro).first;
			BT_Node * curn = (*cur).first;
			if(curn != NULL) curn = (*cur).Rmostchild();

			if(curn != NULL) (*par_rbro).lch = (*curn).rch;
			else{
				(*par_rbro).lch = ls;
				if(ls != NULL) (*ls).par = rbro;
			}
			(*par_rbro).rch = (*(*rbro).first).lch;
			BT_Node * nw = par_rbro;
			(*rbro).sz ++, (*lst).bfr = nw, (*nw).nxt = lst, (*nw).bfr = NULL, (*rbro).first = nw;
			lst = nw, nw = curn;
			while(nw != NULL){
				(*rbro).sz ++, (*lst).bfr = nw, (*nw).nxt = lst, (*rbro).first = nw;
				if((*nw).lch != NULL) (*(*nw).lch).par = rbro;
				if((*nw).rch != NULL) (*(*nw).rch).par = rbro;
				lst = nw, nw = (*nw).bfr;
			}
			nls = rbro;
		}
		if((*(*cur).par).sz < ((T + 1) / 2) - 1) RAR((*cur).par, nls);
		return;
	}
};

class Hash{
public:
	int StoI(string s){
		int ans = 0;
		for(int i = 0; i < (int)s.size(); i++){
			int tmp = 0;
			if(s[i] == '"') tmp = 37;
			else if(s[i] <= '9') tmp = (int)s[i] - 48;
			else tmp = (int)s[i] - 86;
			ans = ((ans * B) + tmp) % MOD;
		}
		while((int)S[ans].size() > 0 && S[ans] != s){
			ans = (ans + 1) % MOD;
		}
		S[ans] = s;
		return ans;
	}
	string ItoS(int ans){
		return S[ans];
	}

private:
	const int MOD = 1e6, B = 38;
	string * S = new string [MOD];
};

bool check(string * inp);

class Table{
public:
	string name = "";
	Table(string* inp){
		Free_ID = new priority_queue <int>;
		while(inp[ccnt] != "#") ccnt ++;
		ccnt = (ccnt - 3) / 2, ccnt ++;
		name = inp[2];
		cname = new string[ccnt];
		ctype = new string[ccnt];
		Badd = new B_Tree * [ccnt];

		cname[0] = "ID";
		ctype[0] = "int";
		Badd[0] = new B_Tree();

		int cur = 3;
		int cnttmp = 1;
		while(inp[cur] != "#"){
			cname[cnttmp] = inp[cur];
			ctype[cnttmp] = inp[cur + 1];
			Badd[cnttmp] = new B_Tree();
			cur += 2;
			cnttmp ++;
		}
		return;
	}

	void Insert(string* inp){
		int id;
		if((*Free_ID).empty()) id = ++rcnt;
		else{
			id = -1 * (*Free_ID).top();
			(*Free_ID).pop();
		}

		Node * lst = new Node(id);
		Node * first = lst;
		(*Badd[0]).Add(lst);
		int cnttmp = 1, cur = 4;
		while(inp[cur] != "#"){
			int tmpint;
			if(ctype[cnttmp] == "int") tmpint = StoI(inp[cur]);
			else if(ctype[cnttmp] == "string") tmpint = H.StoI(inp[cur]);
			else tmpint = FtoD(inp[cur]);

			Node * tmpnode = new Node(tmpint);
			(*Badd[cnttmp]).Add(tmpnode);
			(*lst).nextField = tmpnode;
			lst = tmpnode;
			cnttmp ++, cur ++;
		}
		(*lst).nextField = first;
		return;
	}

	Node_List * Con_Finder(string con){
		Node_List * ans = new Node_List;

		int cur = 0;
		string fname = "";
		while(con[cur] != '<' && con[cur] != '>' && con[cur] != '=') fname += con[cur ++];
		string cond = "";
		while(con[cur] == '<' || con[cur] == '>' || con[cur] == '=') cond += con[cur ++];
		string conn = "";
		while(cur < (int)con.size()) conn += con[cur++];

		int cind = 0;
		while(cind < ccnt && cname[cind] != fname) cind ++;
		if(cind == ccnt) return ans;

		int tmpint;
		if(ctype[cind] == "int") tmpint = StoI(conn);
		else if(ctype[cind] == "string") tmpint = H.StoI(conn);
		else tmpint = FtoD(conn);

		if(cond == "==") return (*Badd[cind]).FindEQ(tmpint);
		else if(cond == "<") (*Badd[cind]).FindSM(tmpint, ans, 1);
		else if(cond == ">") (*Badd[cind]).FindBG(tmpint, ans, 1);
		return ans;
	}

	void Select(string * inp){
		bool * selected = new bool [ccnt];
		for(int i = 0; i < ccnt; i++) selected[i] = 0;
		int bg = 1, en = 1;
		while(inp[en] != "FROM") en ++;
		for(int i = bg; i < en; i++) for(int j = 0; j < ccnt; j++){
			if(inp[i] == cname[j]){
				selected[j] = 1;
				break;
			}
		}
		if(inp[bg] == "*") for(int j = 0; j < ccnt; j++) selected[j] = 1;

		int conind = 0;
		while(inp[conind] != "WHERE") conind ++;
		conind ++;
		Node_List * nodes = Con_Finder(inp[conind]);
		conind ++;
		if(inp[conind] != "#"){
			Node_List * nodes2 = Con_Finder(inp[conind + 1]);
			Node_List * ans = new Node_List;
			for(List_Node * pnt2 = (*nodes2).first; pnt2 != NULL; pnt2 = (*pnt2).nxt){
				(*ans).add((*pnt2).cur);
			}
			for(List_Node * pnt2 = (*nodes).first; pnt2 != NULL; pnt2 = (*pnt2).nxt){
				(*ans).add((*pnt2).cur);
			}
			Sort(ans);

			if(inp[conind] == "&"){
				nodes = new Node_List;
				for(List_Node * pnt2 = (*ans).first; pnt2 != NULL; pnt2 = (*pnt2).nxt){
					if((*pnt2).nxt != NULL && (*(*pnt2).nxt).cur == (*pnt2).cur){
						(*nodes).add((*pnt2).cur);
					}
				}
			}
			else if(inp[conind] == "|"){
				nodes = new Node_List;
				for(List_Node * pnt2 = (*ans).first; pnt2 != NULL; pnt2 = (*pnt2).nxt){
					if((*pnt2).nxt == NULL || (*(*pnt2).nxt).cur != (*pnt2).cur){
						(*nodes).add((*pnt2).cur);
					}
				}
			}
		}
		if(nodes == NULL) return;
		Sort(nodes);

		List_Node * curlnode = (*nodes).first;
		for(int i = 0; i < (*nodes).sz; curlnode = (*curlnode).nxt, i++){
			Node * curnode = (*curlnode).cur;
			for(int j = 0; j < ccnt; j++, curnode = (*curnode).nextField){
				if(selected[j]){
					if(ctype[j] == "int") cout << (*curnode).data << " ";
					else if(ctype[j] == "string"){
						string tmps = H.ItoS((*curnode).data);
						tmps = tmps.substr(1, (int)tmps.size() - 2);
						cout << tmps << " ";
					}
					else cout << DtoF((*curnode).data) << " ";
				}
			}
			cout << '\n';
		}
		return;
	}

	void Delete(string * inp){
		int conind = 0;
		while(inp[conind] != "WHERE") conind ++;
		conind ++;
		Node_List * nodes = Con_Finder(inp[conind]);
		if((*nodes).sz == 0) return;

		Sort(nodes);

		List_Node * curnode = (*nodes).first;
		for(int j = 0; j < (*nodes).sz; j++){
			(*Free_ID).push(-1 * (*(*curnode).cur).data);
			for(int i = 0; i < ccnt; i++){
				(*Badd[i]).Rmv((*curnode).cur);
				(*curnode).cur = (*(*curnode).cur).nextField;
			}
			curnode = (*curnode).nxt;
		}
		return;
	}

	void Update(string * inp){
		int conind = 0;
		while(inp[conind] != "WHERE") conind ++;
		conind ++;
		Node_List * nodes = Con_Finder(inp[conind]);

		Sort(nodes);

		List_Node * curnode = (*nodes).first;
		for(int j = 0; j < (*nodes).sz; j++){
			for(int i = 0; i < ccnt && inp[i + 2] != "WHERE"; i++){
				if(i != 0){
					(*Badd[i]).Rmv((*curnode).cur);

					int tmpint = 0;
					if(ctype[i] == "int") tmpint = StoI(inp[i + 2]);
					else if(ctype[i] == "string") tmpint = H.StoI(inp[i + 2]);
					else tmpint = FtoD(inp[i + 2]);
					(*(*curnode).cur).data = tmpint;

					(*Badd[i]).Add((*curnode).cur);
				}
				(*curnode).cur = (*(*curnode).cur).nextField;
			}
			curnode = (*curnode).nxt;
		}
		return;
	}

private:
	int rcnt = 0, ccnt = 0;
	string * cname = NULL;
	string * ctype = NULL;
	B_Tree ** Badd = NULL;
	priority_queue <int> * Free_ID = NULL;
	Hash H;

	void Sort(Node_List * nodes){
		if(nodes == NULL || (*nodes).sz == 0) return;
		List_Node * curnode = (*nodes).first;
		for(int i = 0; i < (*nodes).sz; i++){
			while(!(*Badd[0]).FindN((*curnode).cur)){
				(*curnode).cur = (*(*curnode).cur).nextField;
			}
			curnode = (*curnode).nxt;
		}

		curnode = (*nodes).first;
		for(int i = 0; i < (*nodes).sz; i++, curnode = (*curnode).nxt){
			List_Node * tmpnode = curnode;
			while((*tmpnode).bfr != NULL && (*(*tmpnode).cur).data < (*(*(*tmpnode).bfr).cur).data){
				Node * tmmp = (*tmpnode).cur;
				(*tmpnode).cur = (*(*tmpnode).bfr).cur;
				(*(*tmpnode).bfr).cur = tmmp;
				tmpnode = (*tmpnode).bfr;
			}
		}

		return;
	}


	int StoI(string s){
		int ans = 0, cur = 0;
		while(cur < (int)s.size()) ans = (ans * 10) + (int)s[cur ++] - 48;
		return ans;
	}

	int FtoD(string s){
		int year = 0, mounth = 0, day = 0, cur = 0;
		while(s[cur] != '/') year = (year * 10) + (int)s[cur ++] - 48;
		cur ++;
		while(s[cur] != '/') mounth = (mounth * 10) + (int)s[cur ++] - 48;
		cur ++;
		while(cur != (int)s.size()) day = (day * 10) + (int)s[cur ++] - 48;
		return ((year * 10000) + (mounth * 100) + day);
	}
	string DtoF(int D){
		int * TMP = new int [3];
		TMP[0] = D / 10000, D %= 10000;
		TMP[1] = D / 100, D %= 100;
		TMP[2] = D;

		string ans = "", tmp = "";
		for(int i = 0; i < 3; i++){
			while(TMP[i] > 0){
				tmp += (char)((TMP[i] % 10) + 48);
				TMP[i] /= 10;
			}
			reverse(tmp.begin(), tmp.end());
			ans += tmp, tmp = "";
			if(i != 2) ans += '/';
		}

		return ans;
	}

	friend bool check(string * inp);
};

vector <Table *> TL;

bool checks(string s){
	if(s[0] != '"' || s[(int)s.size() - 1] != '"') return false;
	for(auto u : s){
		if(u != '"' && !(u >= 'a' && u <= 'z') && !(u >= '0' && u <= '9') && !(u >= 'A' && u <= 'Z')){
			return false;
		}
	}
	return true;
}
bool checki(string s){
	for(auto u : s) if(u < '0' || u > '9'){
		return false;
	}
	return true;
}
bool checkd(string s){
	int cntt = 0, cur = 0;
	for(auto u : s){
		if(u != '/' && (u < '0' || u > '9')) return false;
		if(u != '/') cur = (cur * 10) + (int)u - 48;
		if((cntt == 1 && cur > 12) || (cntt == 2 && cur > 31)) return false;
		if(u == '/') cur = 0, cntt ++;
	}
	return true;
}

bool check(string * inp){
	if(inp[0] == "CREATE"){
		if(inp[1] != "TABLE"){
			cout << "Syntax Error" << endl;
			return false;
		}
		for(auto u : TL) if((*u).name == inp[2]){
			cout << "Table already exists" << endl;
			return false;
		}
		for(int i = 3; inp[i] != "#"; i += 2){
			if(inp[i + 1] == "#"){
				cout << "Syntax Error" << endl;
				return false;
			}
			if(inp[i + 1] != "string" && inp[i + 1] != "timestamp" && inp[i + 1] != "int"){
				cout << "Syntax Error" << endl;
				return false;
			}
		}
	}
	else if(inp[0] == "INSERT"){
		if(inp[1] != "INTO"){
			cout << "Syntax Error" << endl;
			return false;
		}
		Table * T = NULL;
		for(auto u : TL) if((*u).name == inp[2]){
			T = u;
		}
		if(T == NULL){
			cout << "Table doesn't exist" << endl;
			return false;
		}
		if(inp[3] != "VALUES"){
			cout << "Syntax Error" << endl;
			return false;
		}
		int cnt = 1;
		for(int i = 4; inp[i] != "#"; i++, cnt ++){
			if((*T).ctype[cnt] == "string"){
				if(!checks(inp[i])){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if((*T).ctype[cnt] == "int"){
				if(!checki(inp[i])){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else{
				if(!checkd(inp[i])){
					cout << "Invalid type" << endl;
					return false;
				}
			}
		}
		if(cnt != (*T).ccnt){
			cout << "Too much input" << endl;
			return false;
		}
	}
	else if(inp[0] == "DELETE"){
		if(inp[1] != "FROM"){
			cout << "Syntax Error" << endl;
			return false;
		}
		Table * T = NULL;
		for(auto u : TL) if((*u).name == inp[2]){
			T = u;
		}
		if(T == NULL){
			cout << "Table doesn't exist" << endl;
			return false;
		}
		if(inp[3] != "WHERE"){
			cout << "Syntax Error" << endl;
			return false;
		}
		int cnt = 0;
		for(int i = 4; inp[i] != "#"; i++, cnt ++){
			if(cnt == 1){
				if(inp[i] != "&" && inp[i] != "|"){
					cout << "Syntax Error" << endl;
					return false;
				}
				continue;
			}
			bool flag = false;
			string name = "";
			string cond = "";
			string conn = "";
			for(auto u : inp[i]){
				if(u == '<' || u == '>' || u == '=') cond += u, flag = true;
				else if(!flag) name += u;
				else conn += u;
			}

			int cur = -1;
			for(int j = 0; j < (*T).ccnt; j++) if((*T).cname[j] == name){
				cur = j;
				break;
			}
			if(cur < 0){
				cout << "Syntax Error" << endl;
				return false;
			}
			if((*T).ctype[cur] == "string"){
				if(!checks(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if((*T).ctype[cur] == "int"){
				if(!checki(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if(!checkd(conn)){
				cout << "Invalid type" << endl;
				return false;
			}
		}
		if(cnt > 3){
			cout << "Too much input" << endl;
			return false;
		}
	}
	else if(inp[0] == "UPDATE"){
		Table * T = NULL;
		for(auto u : TL) if((*u).name == inp[1]){
			T = u;
		}
		if(T == NULL){
			cout << "Table doesn't exist" << endl;
			return false;
		}
		if(inp[2] != "SET"){
			cout << "Syntax Error" << endl;
			return false;
		}
		int cnt = 1;
		for(int i = 3; inp[i] != "WHERE"; i++, cnt ++){
			if((*T).ctype[cnt] == "string"){
				if(!checks(inp[i])){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if((*T).ctype[cnt] == "int"){
				if(!checki(inp[i])){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if(!checkd(inp[i])){
				cout << "Invalid type" << endl;
				return false;
			}
		}
		if(cnt != (*T).ccnt){
			cout << "Too much input" << endl;
			return false;
		}

		cnt = 0;
		for(int i = 3 + (*T).ccnt; inp[i] != "#"; i++, cnt ++){
			if(cnt == 1){
				if(inp[i] != "&" && inp[i] != "|"){
					cout << "Syntax Error" << endl;
					return false;
				}
				continue;
			}
			bool flag = false;
			string name = "";
			string cond = "";
			string conn = "";
			for(auto u : inp[i]){
				if(u == '<' || u == '>' || u == '=') cond += u, flag = true;
				else if(!flag) name += u;
				else conn += u;
			}

			int cur = -1;
			for(int i = 0; i < (*T).ccnt; i++) if((*T).cname[i] == name){
				cur = i;
				break;
			}
			if(cur < 0){
				cout << "Syntax Error" << endl;
				return false;
			}
			if((*T).ctype[cur] == "string"){
				if(!checks(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if((*T).ctype[cur] == "int"){
				if(!checki(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if(!checkd(conn)){
				cout << "Invalid type" << endl;
				return false;
			}
		}
		if(cnt > 3){
			cout << "Too much input" << endl;
			return false;
		}
	}
	else if(inp[0] == "SELECT"){
		int cur = 1;
		while(inp[cur] != "FROM" && inp[cur] != "#"){
			if(inp[cur] == "*"){
				cur ++;
				break;
			}
			cur ++;
		}
		if(inp[cur] != "FROM"){
			cout << "Syntax Error" << endl;
			return false;
		}
		cur ++;
		Table * T = NULL;
		for(auto u : TL) if((*u).name == inp[cur]){
			T = u;
		}
		if(T == NULL){
			cout << "Table doesn't exist" << endl;
			return false;
		}
		int cur2 = 1;
		while(inp[cur2] != "FROM"){
			if(inp[cur2] == "*") break;
			int tmp = -1;
			for(int i = 0; i < (*T).ccnt; i++) if((*T).cname[i] == inp[cur2]){
				tmp = i;
				break;
			}
			if(tmp < 0){
				cout << "Column not found" << endl;
				return false;
			}
			cur2 ++;
		}
		cur ++;
		if(inp[cur] != "WHERE"){
			cout << "Syntax Error" << endl;
			return false;
		}
		int cnt = 0;
		for(int i = cur + 1; inp[i] != "#"; i++, cnt ++){
			if(cnt == 1){
				if(inp[i] != "&" && inp[i] != "|"){
					cout << "Syntax Error" << endl;
					return false;
				}
				continue;
			}
			bool flag = false;
			string name = "";
			string cond = "";
			string conn = "";
			for(auto u : inp[i]){
				if(u == '<' || u == '>' || u == '=') cond += u, flag = true;
				else if(!flag) name += u;
				else conn += u;
			}

			int cur = -1;
			for(int i = 0; i < (*T).ccnt; i++) if((*T).cname[i] == name){
				cur = i;
				break;
			}
			if(cur < 0){
				cout << "Syntax Error" << endl;
				return false;
			}
			if((*T).ctype[cur] == "string"){
				if(!checks(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if((*T).ctype[cur] == "int"){
				if(!checki(conn)){
					cout << "Invalid type" << endl;
					return false;
				}
			}
			else if(!checkd(conn)){
				cout << "Invalid type" << endl;
				return false;
			}
		}
		if(cnt > 3){
			cout << "Too much input" << endl;
			return false;
		}
	}
	else{
		cout << "Syntax Error" << endl;
		return false;
	}
	return true;
}

void FM(string s){
	int cnt = 0, cntt = 0;
	for(int i = 0; i < (int)s.size(); i++){
		if(s[i] == '(' || s[i] == '(' || s[i] == ',' || s[i] == ' '){
			if(cntt == 0) continue;
			cnt ++, cntt = 0;
		}
		else cntt ++;
	}
	if(cntt) cnt ++;

	string * ans = new string[cnt + 1];
	string tmp = "";
	cnt = 0;
	for(int i = 0; i < (int)s.size(); i++){
		if(s[i] == '(' || s[i] == ')' || s[i] == ',' || s[i] == ' '){
			if((int)tmp.size() == 0) continue;
			ans[cnt ++] = tmp, tmp = "";
		}
		else tmp += s[i];
	}
	if((int)tmp.size() != 0) ans[cnt ++] = tmp, tmp = "";
	ans[cnt] = '#';

	if(!check(ans)) return;

	if(ans[0] == "CREATE"){
		Table * tmptable = new Table(ans);
		TL.push_back(tmptable);
		return;
	}

	Table * tmptable = NULL;

	if(ans[0] == "INSERT"){
		for(auto u : TL){
			if((*u).name == ans[2]){
				tmptable = u;
				break;
			}
		}
		(*tmptable).Insert(ans);
		return;
	}
	if(ans[0] == "SELECT"){
		int nind = 0;
		while(ans[nind] != "FROM") nind ++;
		nind ++;
		for(auto u : TL){
			if((*u).name == ans[nind]){
				tmptable = u;
				break;
			}
		}
		(*tmptable).Select(ans);
		return;
	}
	if(ans[0] == "DELETE"){
		for(auto u : TL){
			if((*u).name == ans[2]){
				tmptable = u;
				break;
			}
		}
		(*tmptable).Delete(ans);
		return;
	}
	if(ans[0] == "UPDATE"){
		for(auto u : TL){
			if((*u).name == ans[1]){
				tmptable = u;
				break;
			}
		}
		(*tmptable).Update(ans);
		return;
	}
}

int main(){
	int n;
	string inp;
	getline(cin, inp);
	n = stoi(inp);

	for(int i = 0; i < n; i++){
		getline(cin, inp);
		FM(inp);
	}

	return 0;
}
