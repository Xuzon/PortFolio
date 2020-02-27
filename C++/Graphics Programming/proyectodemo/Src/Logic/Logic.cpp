#include "Logic.h"
#include "Entity.h"


std::vector<Entity*> Logic::_entities;
uint64_t Logic::_time = -1;
uint64_t Logic::_deltaTime = 0;
Clock Logic::_crono;


bool Logic::Init() {
	_crono.start();
	return true;
}

void Logic::Release() {
	DestroyEntities();
}

void Logic::Tick() {

	auto now = _crono.getMicros();
	if (_time != -1)
		_deltaTime = now - _time;
	_time = now;

	// Actualizamos las entidades y borramos las que nos pidan ser eliminadas.
	// Básicamente queremos hacer un:
	//
	//	for (auto entity : entities)
	//		entity->update();
	//
	// pero Si entity->update() devuelve falso hay que borrarlo del vector entities.
	// Hago las dos cosas a la vez, desplazando hacia atrás los elementos.

	auto r = _entities.begin();
	auto w = _entities.begin();
	for (; r < _entities.end(); ++r) {
		if ((*r)->update()) {
			*w = *r;
			++w;
		}
		else {
			// Esta entidad va al cubo.
			delete *r;
		}
	} // for recorriendo las entidades

	  // Borramos las entradas que han quedado al final.
	_entities.erase(w, _entities.end());

}

void Logic::Render(RenderThread& renderThread) {

	for (auto entity : _entities)
		entity->render(renderThread);
}

void Logic::DestroyEntities() {
	for (auto e : _entities)
		delete e;
	_entities.clear();
}

void Logic::AddEntity(Entity *e) {
	_entities.push_back(e);
}
