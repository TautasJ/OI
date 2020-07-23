/*input
6 6 2
2 2 0 1
3 3 2 0 0
3 2 1 3
4 4 0 3 1 2
5 2 2 1
5 1 5
2 1 1
5 0 0 1 0 0
*/
#pragma GCC optimize ("O3")
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<typename T, typename K>
using ordered_map = tree<T, K, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
vector<vector<bool>>visi;
set<vector<bool>>test;
struct state
{
	bool sugavo = false;
	int sp = -1;
	set<int>p, s;
	bool operator<(const state &o)const
	{
		assert(sugavo == false && o.sugavo == false);
		if (sp != o.sp)
			return sp < o.sp;
		if (p != o.p)
			return p < o.p;
		return s < o.s;
	}
};
state merge(state a, state b)
{
	state c;
	c.sugavo = a.sugavo | b.sugavo;
	if (c.sugavo)
		return c;
	for (int i : a.s)
	{
		for (int j : b.p)
		{
			vector<bool>x = visi[i];
			for (bool g : visi[j])
				x.push_back(g);
			if (test.count(x))
			{
				c.sugavo = true;
				return c;
			}
		}
	}
	if (a.sp != -1 && b.sp != -1)
	{
		vector<bool>x = visi[a.sp];
		for (bool i : visi[b.sp])
			x.push_back(i);
		auto it = lower_bound(visi.begin(), visi.end(), x);
		if (it != visi.end() && (*it) == x)
		{
			c.sp = it - visi.begin();
		}
	}
	c.p = a.p;
	c.s = b.s;
	if (a.sp != -1)
	{
		for (int i : b.p)
		{
			vector<bool>x = visi[a.sp];
			for (bool c : visi[i])
				x.push_back(c);
			auto it = lower_bound(visi.begin(), visi.end(), x);
			if (it != visi.end() && (*it) == x)
			{
				c.p.insert(it - visi.begin());
			}
		}
	}
	if (b.sp != -1)
	{
		for (int i : a.s)
		{
			vector<bool>x = visi[i];
			for (bool c : visi[b.sp])
				x.push_back(c);
			auto it = lower_bound(visi.begin(), visi.end(), x);
			if (it != visi.end() && (*it) == x)
			{
				c.s.insert(it - visi.begin());
			}
		}
	}
	return c;
}
vector<state>visiState;
map<state, int>id;
int add(const state &c)
{
	if (id.count(c) == 0)
	{
		id[c] = visiState.size();
		visiState.push_back(c);
	}
	return id[c];
}
cc_hash_table<ll, int>MM;
int merge(int a, int b)
{
	ll w = a + b * (1ll << 25);
	if (MM.find(w) != MM.end())
		return MM[w];
	state c = merge(visiState[a], visiState[b]);
	if (c.sugavo)
		return MM[w] = -1;
	else
		return MM[w] = add(c);
}
struct Mymap
{
	vector<ull>x;
	ull& operator[](int i)
	{
		while (i >= size())
			x.push_back(-1);
		assert(i >= 0 && i < size());
		return x[i];
	}
	int size()
	{
		return x.size();
	}
};
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0), cout.tie(0);
	int G, N, M;
	cin >> G >> N >> M;
	vector<vector<int>>adj[G];
	for (int i = 0; i < N; i++)
	{
		int a, k;
		cin >> a >> k;
		vector<int>b(k);
		for (int &bi : b)
			cin >> bi;
		adj[a].push_back(b);
	}
	for (int i = 0; i < M; i++)
	{
		int l;
		cin >> l;
		vector<bool>testi(l);
		for (int j = 0; j < l; j++)
		{
			char c;
			cin >> c;
			testi[j] = (c == '1');
		}
		for (int a = 0; a < l; a++)
		{
			vector<bool>x;
			for (int b = a; b < l; b++)
			{
				x.push_back(testi[b]);
				visi.push_back(x);
			}
		}
		test.insert(testi);
	}
	visi.push_back({});
	visi.push_back({false});
	visi.push_back({true});
	sort(visi.begin(), visi.end());
	visi.erase(unique(visi.begin(), visi.end()), visi.end());
	map<vector<bool>, int>Mv;
	for (int i = 0; i < (int)visi.size(); i++)
		Mv[visi[i]] = i;
	if (M == 0)
	{
		ull ans[G];
		fill_n(ans, G, -1ll);
		vector<bool>ok(G, false);
		ok[0] = true;
		ok[1] = true;
		ans[0] = ans[1] = 1;
		bool cikl = true;
		while (cikl)
		{
			cikl = false;
			for (int i = 0; i < G; i++)
			{
				for (vector<int>x : adj[i])
				{
					bool ger = true;
					ull ilg = 0;
					for (int v : x)
					{
						ger &= ok[v];
						ilg += ans[v];
					}
					if (ger && ans[i] > ilg)
					{
						ok[i] = true;
						ans[i] = ilg;
						cikl = true;
					}
				}
			}
		}
		for (int i = 2; i < G; i++)
		{
			if (ok[i])
				cout << "NO " << ans[i] << "\n";
			else
				cout << "YES\n";
		}
		return 0;
	}
	vector<Mymap>Y[G][101];
	Mymap X[G];
	for (bool i : {false, true})
	{
		if (test.count({i}) == 0)
		{
			state s;
			s.sugavo = false;
			s.sp = Mv[ {i}];
			s.p = s.s = {Mv[ {i}], Mv[{}]};
			X[i][add(s)] = 1;
		}
	}
	state nieko;
	nieko.sugavo = false;
	int id0 = Mv[ {}];
	nieko.sp = id0;
	nieko.s = nieko.p = {id0};
	for (int i = 0; i < G; i++)
	{
		for (int j = 0; j < (int)adj[i].size(); j++)
		{
			Y[i][j] = vector<Mymap>(adj[i][j].size());
			Y[i][j][0][add(nieko)] = 0;
		}
	}
	bool ciklas = true;
	while (ciklas)
	{
		ciklas = false;
		for (int i = 2; i < G; i++)
		{
			for (int j = 0; j < (int)adj[i].size(); j++)
			{
				for (int k = 0; k < (int)adj[i][j].size(); k++)
				{
					for (int il = 0; il < Y[i][j][k].size(); il++)
					{
						ull l = Y[i][j][k][il];
						if (l == -1ull)
							continue;
						for (int im = 0; im < X[adj[i][j][k]].size(); im++)
						{
							ull m = X[adj[i][j][k]][im];
							if (m == -1ull)
								continue;
							ull sum = l + m;
							if (sum < (1ull << 63))
							{
								int lm = merge(il, im);
								if (lm != -1)
								{
									Mymap &mp = (k + 1 < (int)adj[i][j].size()) ? Y[i][j][k + 1] : X[i];
									if (mp[lm] != -1ull)
									{
										if (mp[lm] > sum)
										{
											ciklas = true;
											mp[lm] = sum;
										}
									}
									else
									{
										ciklas = true;
										mp[lm] = sum;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	for (int i = 2; i < G; i++)
	{
		ull mn = -1ull;
		for (int j = 0; j < X[i].size(); j++)
			mn = min(mn, X[i][j]);
		if (mn == -1ull)
			cout << "YES\n";
		else
			cout << "NO " << mn << "\n";
	}
	return 0;
}
