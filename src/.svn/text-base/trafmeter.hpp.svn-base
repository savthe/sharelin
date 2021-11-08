/* 
    Sharelin - BSD/Linux terminal gnutella2-client
    Copyright (C) 2008-2011  Andrey Stroganov <savthe@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TRAFMETER_HPP
#define TRAFMETER_HPP

#include <ctime>
#include <stdexcept>
#include <assert.h>

template<typename NumericType>
class TrafMeter
{
public:
	TrafMeter() { reset(); }

	void reset()
	{
		updated_ = 0;
		transferred_ = 0;
		current_ = 0;
		timestamp_ = std::time(0);

		v_[0] = 0;
		v_[1] = 0;
		v_[2] = 0;
	}

	void add(NumericType n)
	{
		timestamp_ = std::time(0);
		current_ += n;
		transferred_ += n;
	}

	NumericType total() const { return transferred_; }
	NumericType rate() const { return (v_[0] + v_[1] + v_[2]) / 3; }
	std::time_t last_activity() const { return timestamp_; }

	void update()
	{
		const std::time_t now = std::time(0);
		if(updated_ == now) return;

		const std::time_t delay = now - updated_;

		switch(delay)
		{
			case 1: v_[0] = v_[1]; v_[1] = v_[2]; v_[2] = current_; break;
			case 2: v_[0] = v_[2]; v_[1] = v_[2] = current_ / 2; break;
			case 3: v_[0] = v_[1] = v_[2] = current_ / 3; break;
			default: v_[0] = v_[1] = v_[2] = 0;
		}

		current_ = 0;

		updated_ = now;

	}

private:
	NumericType v_[3];
	NumericType current_;
	std::time_t updated_;
	NumericType transferred_;
	std::time_t timestamp_;
};

#endif //TRAFMETER_HPP

