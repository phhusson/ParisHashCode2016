#include <assert.h>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>

using namespace std;

class Order;
class Drone;
class Warehouse;
static int nCommands = 0;
static int rows, columns, maxTime, maxLoad;
static std::vector<Drone> drones;
static std::vector<Warehouse> warehouses;
static std::vector<int> productsWeight;
static std::vector<Order> orders;

int distance(int r1, int r2, int c1, int c2) {
    int rDiff = r2 - r1;
    int cDiff = c2 - c1;
    float distance = sqrt(rDiff * rDiff + cDiff * cDiff);
    return ceil(distance);
}

class Order {
	public:
		int id;
		int r, c;
		std::vector<int> products;
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

class Drone {
	public:
		int id;
		int r,c;
		std::list<int> productIds;

		int nTurns;

		//Caching elements
		int weight;
		int distance(Warehouse& w) {
			return ::distance(w.r, w.c, r, c);
		}
		int distance(Order& o) {
			return ::distance(o.r, o.c, r, c);
		}
};

bool droneLoad(int droneNumber, int warehouseId, int productId, int nItems) {
	auto& drone = drones[droneNumber];

	int newNTurns = drone.nTurns;
	newNTurns += drone.distance(warehouses[warehouseId]) + 1;

	if(newNTurns >= maxTime)
		return false;
	drone.nTurns = newNTurns;

	int newWeight = drone.weight;
	newWeight += productsWeight[productId] * nItems;
	if(newWeight >= maxLoad)
		return false;
	drones[droneNumber].weight = newWeight;

	nCommands++;
	cout << droneNumber << " L " << warehouseId << " " << productId << " " << nItems << endl;
	warehouses[warehouseId].loadDrone(productId, nItems);
	return true;

}

bool droneDeliver(int droneNumber, int orderId, int productId, int nItems) {
	auto& drone = drones[droneNumber];

	int newNTurns = drone.nTurns;
	newNTurns += drone.distance(orders[orderId]) + 1;
	if(newNTurns >= maxTime)
		return false;
	drone.nTurns = newNTurns;

	nCommands++;
	drone.weight -= productsWeight[productId] * nItems;
	cout << droneNumber << " D " << orderId << " " << productId << " " << nItems << endl;
	return true;
}

bool droneWait(int droneNumber, int nTurns) {
	auto& drone = drones[droneNumber];

	int newNTurns = drone.nTurns;
	newNTurns += nTurns;
	if(newNTurns >= maxTime)
		return false;
	drone.nTurns = newNTurns;

	nCommands++;
	drone.nTurns += nTurns;
	cout << droneNumber << " W " << nTurns << endl;
	return true;
}

bool droneUnload(int droneNumber, int warehouseId, int productId, int nItems) {
	auto& drone = drones[droneNumber];

	int newNTurns = drone.nTurns;
	newNTurns += drone.distance(warehouses[warehouseId]) + 1;
	if(newNTurns >= maxTime)
		return false;
	drone.nTurns = newNTurns;

	nCommands++;
	drone.weight -= productsWeight[productId] * nItems;
	cout << droneNumber << " U " << warehouseId << " " << productId << " " << nItems << endl;
	warehouses[warehouseId].unloadDrone(productId, nItems);

	return true;
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

	productsWeight.resize(nProducts);
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

	orders.resize(nOrders);
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
		std::sort(orders[i].products.begin(), orders[i].products.end());
	}

    /*
	for(int droneOrder=0; droneOrder<min(nOrders, nDrones); ++droneOrder) {
		int droneId = droneOrder;
		int orderId = droneOrder;
		for(int& product: orders[orderId].products) {
			auto& d = drones[droneId];
			int warehouse = Warehouse::closestProduct(d.r, d.c, product, 1);
			droneLoad(droneId, warehouse, product, 1);
			droneDeliver(droneId, orderId, product, 1);
		}
	}
    */

    int droneId = 0;
	for(int orderId=0; orderId<nOrders; ++orderId) {
		for(int& product: orders[orderId].products) {
			auto& d = drones[droneId];
			int warehouse = Warehouse::closestProduct(d.r, d.c, product, 1);
			droneLoad(droneId, warehouse, product, 1);
			droneDeliver(droneId, orderId, product, 1);
		}
        droneId = (droneId + 1) % nDrones;
	}

	cerr << nCommands << endl;
	return 0;
}
