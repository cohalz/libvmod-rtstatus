#include "cache/cache.h"
#include "vrt.h"
#include "vcl.h"

#include "vmod_rtstatus.h"
#include "vmod_html.h"

VCL_STRING
vmod_html(const struct vrt_ctx *ctx)
{
	struct rtstatus_priv rtstatus = { 0 };

        if (ctx->method != VCL_MET_SYNTH) {
                VSLb(ctx->vsl, SLT_VCL_Error, "rtstatus() can only be used in vcl_synth");
                return "{ \"error\": \"Check Varnishlog for more details\" }";
        }

	rtstatus.vsb = VSB_new(NULL, ctx->ws->f, WS_Reserve(ctx->ws, 0), VSB_AUTOEXTEND);
	VSB_cat(rtstatus.vsb, html);
	VSB_finish(rtstatus.vsb);
	if (VSB_error(rtstatus.vsb)) {
	    VSLb(ctx->vsl, SLT_VCL_Error, "VSB error");
	    WS_Release(ctx->ws, VSB_len(rtstatus.vsb) + 1);
	    return "{ \"error\": \"Check Varnishlog for more details\" }";
	}
	WS_Release(ctx->ws, VSB_len(rtstatus.vsb) + 1);
	return (rtstatus.vsb->s_buf);
}

