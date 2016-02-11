#include <assert.h>
#include <climits>
#include <iostream>
#include <vector>
#include <list>
#include <tuple>
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

int distance2(int r1, int r2, int c1, int c2) {
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
		int score() const;
		std::list<std::pair<int, int>> getProductsWithNumber();
};

class Warehouse {
	public:
		int id;
		int r, c;
		std::vector<int> nProducts;

        static int closestProduct(int r, int c, int productId, int nItems) {
        int bestId = -1;
        int bestDistance = -1;
		for(auto& warehouse: warehouses) {
			if(warehouse.nProducts[productId] < nItems) {
                continue;
			}
            int d = distance2(r, warehouse.r, c, warehouse.c);
            if(bestDistance == -1 || bestDistance > d) {
                bestDistance = d;
                bestId = warehouse.id;
            }
		}
		return bestId;
        }
	
		inline void loadDrone(int productId, int nItems) {
			assert(nItems <= nProducts[productId]);
			nProducts[productId] -= nItems;
		}

		inline void unloadDrone(int productId, int nItems) {
			assert(nItems <= nProducts[productId]);
			nProducts[productId] += nItems;
		}
	int distance(const Order& o) {
		return ::distance(r, o.r, c, o.c);
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
			return ::distance(w.r, r, w.c, c);
		}
		int distance(Order& o) {
			return ::distance(o.r, r, o.c, c);
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

int Order::score() const {
	int score = 0;

	int previous = products[0], nPrevious = 0;
	unsigned i;
	for(i=0; i<products.size(); ++i) {
		if(products[i] != previous) {
			int w = Warehouse::closestProduct(r, c, previous, nPrevious);
			score += warehouses[w].distance(*this);
			nPrevious = 0;
		}
		nPrevious++;
		previous = products[i];
	}
	int w = Warehouse::closestProduct(r, c, previous, nPrevious);
	score += warehouses[w].distance(*this);

	return score;
}

std::list<std::pair<int, int>> Order::getProductsWithNumber() {
	std::list<std::pair<int, int>> ret;
	int previous = products[0], nPrevious = 0;
	for(unsigned i=0; i<products.size(); ++i) {
		if(products[i] != previous) {
			ret.push_back(make_pair(previous, nPrevious));
			nPrevious = 0;
		}
		nPrevious++;
		previous = products[i];
	}
	ret.push_back(make_pair(previous, nPrevious));
	return ret;
}

int main(int argc, char **argv) {
	int nDrones;
	cin >> rows;
	cin >> columns;
	cin >> nDrones;
	drones.resize(nDrones);
	for(int i=0; i<nDrones; ++i)
		drones[i].id = i;

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

	auto orders2 = orders;
	std::sort(orders2.begin(),
			orders2.end(),
			[&](const Order& p, const Order& q) {
				return p.score() < q.score();
			});

	for(auto& order: orders2) {
		auto newDrones = drones;
		// (productId, numberOfProducts)
		std::list<std::pair<int, int>> p = order.getProductsWithNumber();
		
		// v[warehouseId] = list( (productId, numberOfProducts), ... )
		std::vector<std::list<std::pair<int, int>>> pWarehouses(nWarehouses);
		//For each product, chose in which warehouse to take it from
		//Group by warehouse
		for(auto& pp: p) {
			int w = Warehouse::closestProduct(order.r, order.c,
				pp.first, pp.second);
			pWarehouses[w].push_back(pp);
		}

		//For each warehouse
		for(int w=0; w<nWarehouses; ++w) {
			//Search how to full drone
			auto& elements = pWarehouses[w];
			if(elements.size() == 0)
				continue;

			elements.sort(
				[&](std::pair<int, int> a, std::pair<int, int> b) {
					return productsWeight[a.first] > productsWeight[b.first];
				});

			while(true) {
				int nElements = 0;
				for(auto& e: elements) {
					nElements += e.second;
				}
				if(nElements == 0)
					break;

				std::list<int> backpack;
				int weightLeft = maxLoad;
				for(auto& e: elements) {
					while(productsWeight[e.first] < weightLeft && e.second) {
						backpack.push_back(e.first);
						e.second--;
						weightLeft -= productsWeight[e.first];
					}
				}

				//Find closest drone
				int bestDroneDistance = INT_MAX;
				int bestDroneId = -1;
				for(int i=0; i<nDrones; ++i) {
					int d1 = drones[i].distance(warehouses[w]);
					int d2 = warehouses[w].distance(order);
					if(d1 < bestDroneDistance &&
							(maxTime-drones[i].nTurns) > (d1+d2+2) ) {
						bestDroneId = i;
						bestDroneDistance = d1;
					}
				}

				//We got our drone
				for(int b: backpack)
					droneLoad(bestDroneId, w, b, 1);

				for(int b: backpack)
					droneDeliver(bestDroneId, order.id, b, 1);
			}
		}
	}

	for(auto& d: drones) {
		cout << "D: drone " << d.id << ":" << d.nTurns << "/" << maxTime << endl;
	}

	cerr << nCommands << endl;
	return 0;
}
