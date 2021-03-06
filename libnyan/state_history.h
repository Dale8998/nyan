// Copyright 2017-2017 the nyan authors, LGPLv3+. See copying.md for legal info.
#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "config.h"
#include "object_history.h"


namespace nyan {

class Database;
class MetaInfo;
class ObjectState;
class State;



/**
 * Object state history tracking.
 * TODO: merge this class into View.
 */
class StateHistory {
public:
	StateHistory(const std::shared_ptr<Database> &base);

	const std::shared_ptr<State> &get_state_before(order_t t) const;
	const std::shared_ptr<State> *get_state_exact(order_t t) const;

	const std::shared_ptr<ObjectState> *get_obj_state(const fqon_t &fqon, order_t t) const;

	void insert(std::shared_ptr<State> &&new_state, order_t t);

	void insert_linearization(std::vector<fqon_t> &&ins, order_t t);
	const std::vector<fqon_t> &get_linearization(const fqon_t &obj, order_t t,
	                                             const MetaInfo &meta_info) const;

	void insert_children(const fqon_t &obj, std::unordered_set<fqon_t> &&ins, order_t t);
	const std::unordered_set<fqon_t> &get_children(const fqon_t &obj, order_t t,
	                                               const MetaInfo &meta_info) const;

protected:
	ObjectHistory *get_obj_history(const fqon_t &obj);
	const ObjectHistory *get_obj_history(const fqon_t &obj) const;
	ObjectHistory &get_create_obj_history(const fqon_t &obj);

	/**
	 * Storage of states over time.
	 */
	Curve<std::shared_ptr<State>> history;

	/**
	 * Information history for each object.
	 * Optimizes searches in the history.
	 */
	std::unordered_map<fqon_t, ObjectHistory> object_obj_hists;
};


} // namespace nyan
