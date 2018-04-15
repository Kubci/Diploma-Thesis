#include "RadixSort.h"

void RadixSort(std::vector<PixelDataCarrier*>& data_in, std::vector<PixelDataCarrier*>& data_sorted, int sortVal_bitDepth)
{
	std::vector<PixelDataCarrier*> zero_val(data_in.size());
	std::vector<PixelDataCarrier*> zero_val2(data_in.size());
	std::vector<PixelDataCarrier*> one_val(data_in.size());
	std::vector<PixelDataCarrier*> one_val2(data_in.size());

	std::vector<PixelDataCarrier*>& zv = zero_val;
	std::vector<PixelDataCarrier*>& zv2 = zero_val2;
	std::vector<PixelDataCarrier*>& ov = one_val;
	std::vector<PixelDataCarrier*>& ov2 = one_val2;
	std::vector<PixelDataCarrier*>& tmp = zero_val;;

	unsigned position = 1;

	int zero_counter = 0;
	int one_counter = 0;
	int zero_counter2 = 0;
	int one_counter2 = 0;
	int zero_pos = 0;
	int one_pos = 0;

	for (PixelDataCarrier* dtc : data_in)
	{
		unsigned flag = dtc->sort_value & position;
		if (flag)
		{
			ov[one_pos] = dtc;
			one_counter++;
			one_pos++;
		}
		else
		{
			zv[zero_pos] = dtc;
			zero_counter++;
			zero_pos++;
		}
	}

	zero_pos = 0;
	one_pos = 0;

	for (int i = 1; i < sortVal_bitDepth; i++)
	{
		position <<= 1;

		for (int j = 0; j < zero_counter; j++)
		{
			unsigned flag = zv[j]->sort_value & position;
			if (flag)
			{
				ov2[one_pos] = (zv[j]);
				one_counter2++;
				one_pos++;
			}
			else
			{
				zv2[zero_pos] = (zv[j]);
				zero_counter2++;
				zero_pos++;
			}
		}

		for (int j = 0; j < one_counter; j++)
		{
			unsigned flag = ov[j]->sort_value & position;
			if (flag)
			{
				ov2[one_pos] = (ov[j]);
				one_counter2++;
				one_pos++;
			}
			else
			{
				zv2[zero_pos] = (ov[j]);
				zero_counter2++;
				zero_pos++;
			}
		}

		tmp = ov; ov = ov2; ov2 = tmp;
		tmp = zv; zv = zv2; zv2 = tmp;

		one_counter = one_counter2;
		one_counter2 = 0;
		zero_counter = zero_counter2;
		zero_counter2 = 0;
		zero_pos = 0;
		one_pos = 0;
	}

	if (data_sorted.size() == 0)
	{
		for (int j = 0; j < zero_counter; j++)
		{
			data_sorted.push_back(zv[j]);
		}

		for (int j = 0; j < one_counter; j++)
		{
			data_sorted.push_back(ov[j]);
		}
	}
	else
	{
		for (int j = 0; j < zero_counter; j++)
		{
			data_sorted[j] = zv[j];
		}

		for (int j = 0; j < one_counter; j++)
		{
			data_sorted[zero_counter + j] = ov[j];
		}
	}
}

