/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "geometrie.h"

bool_t
xdr_point (XDR *xdrs, point *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->x))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_rectangle (XDR *xdrs, rectangle *objp)
{
	register int32_t *buf;

	 if (!xdr_point (xdrs, &objp->p1))
		 return FALSE;
	 if (!xdr_point (xdrs, &objp->p2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_coordonnees (XDR *xdrs, coordonnees *objp)
{
	register int32_t *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->x1))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->x2))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->y1))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->y2))
				 return FALSE;
		} else {
			IXDR_PUT_LONG(buf, objp->x1);
			IXDR_PUT_LONG(buf, objp->x2);
			IXDR_PUT_LONG(buf, objp->y1);
			IXDR_PUT_LONG(buf, objp->y2);
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, 4 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->x1))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->x2))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->y1))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->y2))
				 return FALSE;
		} else {
			objp->x1 = IXDR_GET_LONG(buf);
			objp->x2 = IXDR_GET_LONG(buf);
			objp->y1 = IXDR_GET_LONG(buf);
			objp->y2 = IXDR_GET_LONG(buf);
		}
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->x1))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->x2))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y1))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y2))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_param_inclus (XDR *xdrs, param_inclus *objp)
{
	register int32_t *buf;

	 if (!xdr_rectangle (xdrs, &objp->rect))
		 return FALSE;
	 if (!xdr_point (xdrs, &objp->p))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_booleen (XDR *xdrs, booleen *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, objp))
		 return FALSE;
	return TRUE;
}
