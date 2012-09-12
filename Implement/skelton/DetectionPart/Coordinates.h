
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>

#include "DetectionPart/Notice.h"
#include "DetectionPart/LocationInfo.h"

namespace DetectionPart
{
/**
 * 現在座標
 */
class Coordinates
{
private:
	/**
	 * 指定x座標
	 */
	float targXCoo;

	/**
	 * 指定y座標
	 */
	float targYCoo;

	Notice notice;
	LocationInfo locationInfo;

public:
	/**
	 * 指定座標移動を検知する
	 */
	bool Coordinates_detecTargCoo(Coordinates *this_Coordinates);

	bool 指定x座標移動を検知する();

	bool 指定y座標移動を検知する();

};

}  // namespace DetectionPart
