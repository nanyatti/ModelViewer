struct VSInput {
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};

cbuffer ConstantBuffer {
	float4x4 World;         //���[���h�ϊ��s��
	float4x4 View;          //�r���[�ϊ��s��
	float4x4 Projection;    //�����ˉe�ϊ��s��
}

VSOutput Main(VSInput input) {
	VSOutput output;

	output.pos = mul(input.pos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.col = input.col;
	return output;
}