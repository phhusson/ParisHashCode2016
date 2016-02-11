#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Order;
class Drone;
class Warehouse;
static int nCommands = 0;
static int rows, columns, maxTime, maxLoad;
static std::vector<Drone> drones;
static std::vector<Warehouse> warehouses;

class Order {
	public:
		int r, c;
		std::vector<int> products;
};

class Drone {
	public:
		int r,c;
		std::list<int> productIds;
		
		int nTurns;
};

class Warehouse {
	public:
		int r, c;
		std::vector<int> nProducts;

        static int closestProduct(int r, int c, int productId, int nItems) {
            for(unsigned i = 0; i < warehouses.size(); ++i) {
                if(warehouses[i].nProducts[productId] >= nItems) {
                    return i;
                }
            }
        }
};

void droneLoad(int droneNumber, int warehouseId, int productId, int nItems) {
	nCommands++;
	drones[droneNumber].nTurns++;
	cout << droneNumber << " L " << warehouseId << " " << productId << " " << nItems << endl;
}

void droneDeliver(int droneNumber, int orderId, int productId, int nItems) {
	nCommands++;
	drones[droneNumber].nTurns++;
	cout << droneNumber << " D " << orderId << " " << productId << " " << nItems << endl;
}

void droneWait(int droneNumber, int nTurns) {
	nCommands++;
	drones[droneNumber].nTurns++;
	cout << droneNumber << " W " << nTurns << endl;
}

void droneUnload(int droneNumber, int warehouseId, int productId, int nItems) {
	nCommands++;
	drones[droneNumber].nTurns++;
	cout << droneNumber << " U " << warehouseId << " " << productId << " " << nItems << endl;
}

int main(int argc, char **argv) {
	int nDrones;
	cin >> rows;
	cin >> columns;
	cin >> nDrones;
	drones.resize(nDrones);

	cin >> maxTime;
	cin >> maxLoad;

	int nProducts;
	cin >> nProducts;

	std::vector<int> productsWeight(nProducts);
	for(int i=0; i<nProducts; ++i)
		cin >> productsWeight[i];

	int nWarehouses;
	cin >> nWarehouses;
	warehouses.resize(nWarehouses);

	std::vector<std::pair<int, int> > warehousesPosition(nWarehouses);
	for(int i=0; i<nWarehouses; ++i) {
		int r, c;
		cin >> r >> c;
		warehouses[i].r = r;
		warehouses[i].c = c;
		
                warehouses[i].nProducts.resize(nProducts);
		for(int j=0; j<nProducts; ++j) {
			cin >> warehouses[i].nProducts[j];
		}
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
		for(int j=0; j<nProducts; ++j) {
			int p;
			cin >> p;
			orders[i].products[j] = p;
		}
	}

#if 0
	droneLoad(0, 1, 2, 3);
	droneDeliver(0, 1, 2, 3);
	droneWait(1, 3);
	droneUnload(1, 1, 1, 1);
	endTurn();
#endif
	
	
	cerr << nCommands << endl;
	return 0;
}
