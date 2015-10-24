#pragma once

class distanciaId
{
private:
public:
	double distancia;
	int id;
	int padre;
	distanciaId() { distancia = -1; id = 0; }
	void setDistancia(double _distancia) { distancia = _distancia; }
	void setId(int _id) { id = _id; }
	void setPadre(int _padre) { padre = _padre; }
	double getDistancia() { return distancia; }
	int getId() { return id; }
	int getPadre() { return padre; }
};