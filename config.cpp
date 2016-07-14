//
// Created by weitao on 7/14/16.
//

#include "config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace snow
{
    config& config::instance() {
        static config config_instance;
        return config_instance;
    }
}