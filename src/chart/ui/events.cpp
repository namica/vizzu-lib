#include "events.h"

#include "base/io/log.h"
#include "base/text/smartstring.h"

using namespace Vizzu;
using namespace Vizzu::UI;

ClickEvent::ClickEvent(const Diag::Marker *marker, Chart &chart) :
    Util::EventDispatcher::Params(chart),
    marker(marker)
{
}

std::string ClickEvent::dataToJson() const
{
	if (marker)
	{
		auto &chart = dynamic_cast<Vizzu::Chart&>(sender);
		auto diagram = chart.getDiagram();
		if (diagram)
		{
			auto cell = diagram->getDataCube().cellInfo(marker->index);

			auto categories = Text::SmartString::map(cell.categories,
			[](const auto &pair)
			{
				auto key = Text::SmartString::escape(pair.first, "\"\\");
				auto value = Text::SmartString::escape(pair.second, "\"\\");
				return "\"" + key + "\":\"" + value + "\"";
			});

			auto values = Text::SmartString::map(cell.values,
			[](const auto &pair)
			{
				auto key = Text::SmartString::escape(pair.first, "\"\\");
				auto value = std::to_string(pair.second);
				return "\"" + key + "\":" + value;
			});

			return
			"{"
				"\"marker\":"
				"{"
					"\"categories\":{"
						+ Text::SmartString::join(categories, ",") +
					"},"
					"\"values\":{"
						+ Text::SmartString::join(values, ",") +
					"}"
				"}"
			"}";
		}
		else return "null";
	} else return "null";
}
