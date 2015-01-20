/*
103 - Stacking Boxes
http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=39
*/
#include <cstdint>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <memory>

class Box : public std::enable_shared_from_this<Box>
{
public:
	//Method used in the sets below (~unsafe implementation)
	static bool comparator(std::weak_ptr<Box> lhs, std::weak_ptr<Box> rhs) { return lhs.lock()->id < rhs.lock()->id; }
	bool operator<(const Box& rhs) const { return id < rhs.id; }

	//The id for the final output and comparison
	uint32_t id;
	//The length for longest path calculation (used later on)
	uint32_t path_length {};
	//The dimensions of the box
	std::vector<uint32_t> dimensions;
	//boxes that fits in
	std::set<std::weak_ptr<Box>, decltype(&Box::comparator)> inner_boxes;
	//boxes where the current box fits in
	std::set<std::weak_ptr<Box>, decltype(&Box::comparator)> outer_boxes;

	//Constructor
	Box(uint32_t id_, std::vector<uint32_t>&& dimensions_) : id(id_), dimensions(std::move(dimensions_)), 
		inner_boxes(&Box::comparator), outer_boxes(&Box::comparator) 
	{
		//sort the vector for further treatments
		std::sort(dimensions.begin(), dimensions.end(), [](uint32_t lhs, uint32_t rhs) { return lhs > rhs; });
	}

	//Checks whether the box can be nested in the bigger box
	bool is_nested(const Box& bigger_box)
	{
		for(auto it = dimensions.cbegin(), it_big = bigger_box.dimensions.cbegin(), end = dimensions.cend(); it != end; ++it, ++it_big)
		{
			if(*it_big <= *it)
				return false;
		}
		return true;
	}

	//Updates nesting relationships with provided boxes
	void find_nesting(std::vector<std::shared_ptr<Box> >& boxes)
	{
		for(auto& box : boxes)
		{
			if(is_nested(*box))
			{
				outer_boxes.insert(box);
				box->inner_boxes.insert(shared_from_this());
			}
		}
	}
};

std::vector<std::shared_ptr<Box> > topological_ordering(std::vector<std::shared_ptr<Box> >&& boxes)
{
	std::vector<std::shared_ptr<Box> > sorted {};
	sorted.reserve(boxes.size());
	std::set<std::shared_ptr<Box> > to_compute {};

	//Add the nodes/boxes with no incoming nodes/inner boxes inside the set
	for(auto box : boxes)
		if(box->inner_boxes.empty())
			to_compute.insert(box);

	//Algorithm loop
	while(!to_compute.empty())
	{
		//Remove first element from to_compute and put it on tail of sorted
		sorted.push_back(*to_compute.begin());
		to_compute.erase(to_compute.begin(), ++to_compute.begin());

		//Go through the outer node of the previous handled element and remove edges going out
		for(auto outer : sorted.back()->outer_boxes)
		{
			auto sp = outer.lock();//we know they still exist in the current scope
			sp->inner_boxes.erase(sp->inner_boxes.find(sorted.back()));
			if(sp->inner_boxes.empty())
				to_compute.insert(sp);//'moved' from boxes to to_compute
		}
	}

	return sorted;
}

void innerize(std::vector<std::shared_ptr<Box> >& boxes)
{
	for(auto& box : boxes)
	{
		for(auto& outer : box->outer_boxes)
		{
			auto sp = outer.lock();
			sp->inner_boxes.insert(box);
		}
	}
}

void path_lengths(std::vector<std::shared_ptr<Box> >& boxes)
{
	//The box must be ordered in a typological order prior to use of this function
	for(auto& box : boxes)
	{
		if(box->inner_boxes.empty())
		{
			box->path_length = 0;
		}
		else
		{
			uint32_t current_max { 0 };
			for(auto& inner : box->inner_boxes)
			{
				auto sp = inner.lock();
				current_max = std::max(sp->path_length, current_max);
			}
			box->path_length = 1+current_max;
		}
	}
}

void display_longest_path(std::vector<std::shared_ptr<Box> >& boxes)
{
	std::vector<uint32_t> answer_ids;
	//Start from the end
	auto start = std::max_element(boxes.begin(), boxes.end(), 
		[](const std::shared_ptr<Box>& lhs, const std::shared_ptr<Box>& rhs) {
			return lhs->path_length < rhs->path_length; 
	});
	std::shared_ptr<Box> current = *start;
	answer_ids.reserve(current->path_length);
	answer_ids.push_back(current->id);

	//Come back till a starting node (little box)
	while(!current->inner_boxes.empty())
	{
		current = (*std::max_element(current->inner_boxes.begin(), current->inner_boxes.end(), 
		[](const std::weak_ptr<Box>& lhs, const std::weak_ptr<Box>& rhs) {
			return lhs.lock()->path_length < rhs.lock()->path_length; 
		})).lock();
		answer_ids.push_back(current->id);
	}

	//Displays results
	std::cout << answer_ids.size() << std::endl;
	for (uint32_t i = answer_ids.size(); i-- > 1; ) {
		std::cout << answer_ids[i] << " ";
	}
	std::cout << answer_ids[0] << std::endl;
}

int main(void)
{
	std::ios::sync_with_stdio(false);

	uint32_t amount_boxes, amount_dimensions, dimension;
	std::vector<uint32_t> dimensions;
	std::vector<std::shared_ptr<Box> > boxes;
	do
	{
		//Gets inputs
		std::cin >> amount_boxes;
		if(std::cin.eof())
			break;
		boxes.reserve(amount_boxes);
		std::cin >> amount_dimensions;
		dimensions.reserve(amount_dimensions);
		for(uint32_t i = 0; i < amount_boxes; ++i)
		{
			for(uint32_t j = 0; j < amount_dimensions; ++j)
			{
				std::cin >> dimension;
				dimensions.push_back(dimension);
			}
			boxes.push_back(std::make_shared<Box>(i+1, std::move(dimensions)));
		}

		//Creates the graph
		for(auto& box : boxes)
			box->find_nesting(boxes);
		//Topological ordering
		boxes = topological_ordering(std::move(boxes));
		//Restores inner links
		innerize(boxes);
		//Computes length for finding longest path
		path_lengths(boxes);

		//Finds path and displays result
		display_longest_path(boxes);

		boxes.clear();
	} while(1);
}