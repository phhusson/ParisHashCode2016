#include <iostream>
#include <vector>

using namespace std;

class Order {
	public:
		int r, c;
		std::vector<int> products;
};

int main(int argc, char **argv) {
	int rows, columns, drones, maxTime, maxLoad;

	cin >> rows;
	cin >> columns;
	cin >> drones;
	cin >> maxTime;
	cin >> maxLoad;

	int nProducts;
	cin >> nProducts;

	std::vector<int> productsWeight(nProducts);
	for(int i=0; i<nProducts; ++i)
		cin >> productsWeight[i];

	int nWarehouses;
	cin >> nWarehouses;

	std::vector<std::pair<int, int> > warehousesPosition(nWarehouses);
	for(int i=0; i<nWarehouses; ++i) {
		int r, c;
		cin >> r >> c;
		warehousesPosition[i] = std::make_pair(r, c);
	}

	int nOrders;
	cin >> nOrders;

	std::vector<Order> orders(nOrders);
	for(int i=0; i<nOrders; ++i) {
		int r, c;
		cin >> r >>c;
		
		orders[i].r = r;
		orders[i].c = c;

		int nProducts;
		cin >> nProducts;

		orders[i].products.resize(nProducts);
		for(int j=0; j<nOrders; ++j) {
			int p;
			cin >> p;
			orders[i].products[j] = p;
		}
	}


	return 0;
}
