#pragma once

namespace ishihalib {

namespace motor_spec {

struct motorSpec_t {
	static constexpr float R = 0;	//抵抗
	static constexpr float Kv = 0;	//(V)/(rps)
	static constexpr float L = 0;	//インダクタ

	static constexpr float Kt = 0;	//Nm/A
};

struct RZ735VA : public motorSpec_t {
	static constexpr float R = 18. / 156;						 //抵抗
	static constexpr float Kv = (18. - R * 2.8) / (20400 / 60);	 //(V)/(rps)
	static constexpr float L = 0;								 //インダクタ

	static constexpr float Kt = 1.265 / 156.;  //Nm/A
};

};	// namespace motor_spec

};	// namespace ishihalib
