#include <assert.h>
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
		int id;
		int r, c;
		std::vector<int> products;
};

class Drone {
	public:
		int id;
		int r,c;
		std::list<int> productIds;
		
		int nTurns;
};

class Warehouse {
	public:
		int id;
		int r, c;
		std::vector<int> nProducts;

        static int closestProduct(int r, int c, int productId, int nItems) {
		for(auto& warehouse: warehouses) {
			if(warehouse.nProducts[productId] >= nItems) {
			    return warehouse.id;
			}
		}
		return -1;
        }
	
		inline void loadDrone(int productId, int nItems) {
			assert(nItems <= nProducts[productId]);
			nProducts[productId] -= nItems;
		}

		inline void unloadDrone(int productId, int nItems) {
			assert(nItems <= nProducts[productId]);
			nProducts[productId] += nItems;
		}
};

void droneLoad(int droneNumber, int warehouseId, int productId, int nItems) {
	nCommands++;
	drones[droneNumber].nTurns++;
	cout << droneNumber << " L " << warehouseId << " " << productId << " " << nItems << endl;
	warehouses[warehouseId].loadDrone(productId, nItems);
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
	warehouses[warehouseId].unloadDrone(productId, nItems);
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
		warehouses[i].id = i;
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
		orders[i].id = i;

		orders[i].products.resize(nProducts);
		for(int j=0; j<nProducts; ++j) {
			int p;
			cin >> p;
			orders[i].products[j] = p;
		}
	}

	int droneId = 0;
	int orderId = 0;
	for(int& product: orders[orderId].products) {
		auto& d = drones[droneId];
		int warehouse = Warehouse::closestProduct(d.r, d.c, product, 1);
		droneLoad(droneId, warehouse, product, 1);
		droneDeliver(droneId, orderId, products, 1);
	}
	
	cerr << nCommands << endl;
	return 0;
}
