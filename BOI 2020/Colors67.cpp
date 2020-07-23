/*input
2
1
5
0
0
0
0
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
map<pair<ll, pair<vector<ll>, vector<ll>>>, ll>M;
ll ask(ll n, vector<ll>x, vector<ll>y)
{
	pair<ll, pair<vector<ll>, vector<ll>>> par = {n, {x, y}};
	if (M.count(par))
		return M[par];
	assert(x.size() < n);
	assert(x.size() == y.size());
	if (n == 1)
		return M[par] = 1;
	if (n == 2)
	{
		if (x.empty())
			return M[par] = 1;
		else
			return M[par] = 2;
	}
	if (n == 3)
	{
		if (x.empty())
			return M[par] = 1;
		if (x.size() == 1)
			return M[par] = 3;
		return M[par] = 2;
	}
	if (n % 2 == 0)
	{
		ll j = n / 2;
		ll f = ask(j, {}, {});
		ll g = ask(j, {f}, {0});
		bool sw = g < f;
		if (sw)
			f = j - f + 1;
		if (x.empty())
			return M[par] = f + j;
		if (x.size() == 1)
			return M[par] = f;
		ll past = y[1];
		x.erase(x.begin());
		y.erase(y.begin());
		if (past == 1)
		{
			if (sw)
			{
				for (ll &i : x)
					i = j - i + 1;
				return M[par] = j - ask(n / 2, x, y) + 1;
			}
			else
				return M[par] = ask(n / 2, x, y);
		}
		else
		{
			vector<ll>del;
			for (ll i = 1; i < x.size(); i++)
			{
				ll v = x[i] - x[i - 1];
				if (v > 0)
					v -= j;
				else
					v += j;
				del.push_back(v);
			}
			vector<ll>xc = {f};
			for (ll i = 0; i < (ll)del.size(); i++)
				xc.push_back(xc.back() + del[i]);
			assert(xc.size() == y.size());
			if (sw)
				for (ll &i : xc)
					i = j - i + 1;
			ll val = ask(j, xc, y);
			if (sw)
				val = j - val + 1;
			if (sw)
				xc.back() = j - xc.back() + 1;
			ll dx = val - xc.back();
			if (dx > 0)
				dx += j;
			else
				dx -= j;
			return M[par] = x.back() + dx;
		}
	}
	else
	{
		bool viens = false;
		{
			vector<ll>a;
			vector<ll>b;
			while (true)
			{
				ll x = ask(n - 1, a, b);
				if (x == 1)
				{
					viens = true;
					break;
				}
				else if (x == n - 1)
					break;
				a.push_back(x);
				b.push_back(0);
			}
		}
		ll lo = 1;
		ll hi = n;
		for (int i = 1; i < (int)x.size(); i++)
		{
			if (y[i] == 1)
				hi = min(hi, abs(x[i - 1] - x[i]));
			else
				lo = max(lo, abs(x[i - 1] - x[i]) + 1);
		}
		assert(lo < hi);
		ll del = viens ? 1 : 0;
		if (hi == n && lo == n - 1)
		{
			if (del == 0)
				return M[par] = n;
			else
				return M[par] = 1;
		}
		for (ll &i : x)
			i -= del;
		return M[par] = ask(n - 1, x, y) + del;
	}
}
void test()
{
	for (int n = 1; n <= 1000; n++)
	{
		for (int c = 1; c <= n; c++)
		{
			cout << n << " " << c << endl;
			vector<ll>x;
			vector<ll>ans;
			ll lo = 1;
			ll hi = n;
			ll last = -1;
			while (lo < hi)
			{
				ll v = ask(n, x, ans);
				cout << "? " << v << endl;
				int k = 0;
				if (last != -1 && abs(last - v) >= c)
					k = 1;
				x.push_back(v);
				ans.push_back(k);
				if (last != -1)
				{
					if (k == 1)
						hi = min(hi, abs(last - v));
					else
						lo = max(lo, abs(last - v) + 1);
				}
				last = v;
			}
			if ((lo + hi) / 2 != c)
			{
				cout << n << ", " << c << " " << (lo + hi) / 2 << endl;
			}
			else
				cout << "ok\n";
		}
	}
}
int main()
{
	//test();
	ios_base::sync_with_stdio(false), cin.tie(0), cout.tie(0), cerr.tie(0);
	ll t;
	cin >> t;
	while (t--)
	{
		ll n;
		cin >> n;
		vector<ll>x;
		vector<ll>ans;
		ll lo = 1;
		ll hi = n;
		ll last = -1;
		while (lo < hi)
		{
			ll v = ask(n, x, ans);
			cout << "? " << v << endl;
			ll k;
			cin >> k;
			x.push_back(v);
			ans.push_back(k);
			if (last != -1)
			{
				if (k == 1)
					hi = min(hi, abs(last - v));
				else
					lo = max(lo, abs(last - v) + 1);
			}
			last = v;
			if (x.size() == 64)
				break;
		}
		cout << "= " << (lo + hi) / 2 << endl;
	}
}
