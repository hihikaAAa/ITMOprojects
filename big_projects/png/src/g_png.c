#include "../include/lib_usage.h"
#include "../include/g_png.h"
#include "../include/helper_struct.h"
#include <stdlib.h>
#include <string.h>
#include <png.h>

int release_png_context(PngContext *ctx, int error_code)
{
	if (ctx->row_pointers)
	{
		for (int y = 0; y < ctx->height; y++)
		{
			free(ctx->row_pointers[y]);
		}
		free(ctx->row_pointers);
		ctx->row_pointers = NULL;
	}

	if (ctx->palette)
	{
		free(ctx->palette);
		ctx->palette = NULL;
	}

	if (ctx->png_ptr || ctx->info_ptr)
	{
		png_destroy_read_struct(&ctx->png_ptr, &ctx->info_ptr, NULL);
	}

	if (ctx->fp)
	{
		fclose(ctx->fp);
		ctx->fp = NULL;
	}

	if (error_code != PNG_OK)
	{
		if (error_code < 0 || error_code > PNG_ERROR_UNKNOWN)
			error_code = PNG_ERROR_UNKNOWN;

		fprintf(stderr,
		        "Error code %d: %s\n",
		        error_code,
		        png_error_message_table[error_code]);
	}

	return error_code;
}

bool read_png(
	const char *filename,
	png_bytep **rows,
	int *width,
	int *height,
	int *color_type,
	int *bit_depth,
	png_colorp *palette,
	int *num_palette
	)
{
	*rows = NULL;
	*palette = NULL;
	*num_palette = 0;

	PngContext ctx;
	memset(&ctx, 0, sizeof(ctx));

	int error_code = PNG_OK;

	ctx.fp = fopen(filename, "rb");
	if (!ctx.fp)
	{
		error_code = PNG_ERROR_CANNOT_OPEN_FILE;
		release_png_context(&ctx, error_code);
		return false;
	}

	unsigned char header[8];
	if (fread(header, 1, 8, ctx.fp) != 8)
	{
		error_code = PNG_ERROR_FILE_TOO_SHORT;
		release_png_context(&ctx, error_code);
		return false;
	}
	if (png_sig_cmp(header, 0, 8))
	{
		error_code = PNG_ERROR_NOT_A_PNG;
		release_png_context(&ctx, error_code);
		return false;
	}

	ctx.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!ctx.png_ptr)
	{
		error_code = PNG_ERROR_CREATE_READ_STRUCT;
		release_png_context(&ctx, error_code);
		return false;
	}

	ctx.info_ptr = png_create_info_struct(ctx.png_ptr);
	if (!ctx.info_ptr)
	{
		error_code = PNG_ERROR_CREATE_INFO_STRUCT;
		release_png_context(&ctx, error_code);
		return false;
	}

	if (setjmp(png_jmpbuf(ctx.png_ptr)))
	{
		error_code = PNG_ERROR_READ;
		release_png_context(&ctx, error_code);
		return false;
	}

	png_init_io(ctx.png_ptr, ctx.fp);
	png_set_sig_bytes(ctx.png_ptr, 8);
	png_read_info(ctx.png_ptr, ctx.info_ptr);

	*width = png_get_image_width(ctx.png_ptr, ctx.info_ptr);
	*height = png_get_image_height(ctx.png_ptr, ctx.info_ptr);
	*color_type = png_get_color_type(ctx.png_ptr, ctx.info_ptr);
	*bit_depth = png_get_bit_depth(ctx.png_ptr, ctx.info_ptr);

	ctx.height = *height;

	if (*bit_depth != 8)
	{
		error_code = PNG_ERROR_INVALID_BIT_DEPTH;
		release_png_context(&ctx, error_code);
		return false;
	}

	if (*color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
	    *color_type == PNG_COLOR_TYPE_RGBA ||
	    *color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		error_code = PNG_ERROR_UNSUPPORTED_COLOR_TYPE;
		release_png_context(&ctx, error_code);
		return false;
	}

	if (*color_type != PNG_COLOR_TYPE_GRAY &&
	    *color_type != PNG_COLOR_TYPE_RGB &&
	    *color_type != PNG_COLOR_TYPE_PALETTE)
	{
		error_code = PNG_ERROR_UNSUPPORTED_COLOR_TYPE;
		release_png_context(&ctx, error_code);
		return false;
	}

	if (*color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_colorp pal;
		int num;
		png_get_PLTE(ctx.png_ptr, ctx.info_ptr, &pal, &num);
		if (pal && num > 0 && num <= 256)
		{
			ctx.palette = (png_colorp)malloc(sizeof(png_color) * num);
			if (!ctx.palette)
			{
				error_code = PNG_ERROR_UNKNOWN;
				release_png_context(&ctx, error_code);
				return false;
			}
			memcpy(ctx.palette, pal, sizeof(png_color) * num);
			*palette = ctx.palette;
			*num_palette = num;
		}
		else
		{
			error_code = PNG_ERROR_INVALID_PALETTE;
			release_png_context(&ctx, error_code);
			return false;
		}
	}

	png_read_update_info(ctx.png_ptr, ctx.info_ptr);

	ctx.row_pointers = (png_bytep *)malloc((*height) * sizeof(png_bytep));
	if (!ctx.row_pointers)
	{
		error_code = PNG_ERROR_UNKNOWN;
		release_png_context(&ctx, error_code);
		return false;
	} {
		int rowbytes = png_get_rowbytes(ctx.png_ptr, ctx.info_ptr);
		for (int y = 0; y < *height; y++)
		{
			ctx.row_pointers[y] = (png_byte *)malloc(rowbytes);
			if (!ctx.row_pointers[y])
			{
				error_code = PNG_ERROR_UNKNOWN;
				release_png_context(&ctx, error_code);
				return false;
			}
		}
	}

	png_read_image(ctx.png_ptr, ctx.row_pointers);

	*rows = ctx.row_pointers;
	ctx.row_pointers = NULL;

	png_destroy_read_struct(&ctx.png_ptr, &ctx.info_ptr, NULL);
	if (ctx.fp)
	{
		fclose(ctx.fp);
		ctx.fp = NULL;
	}

	return true;
}

bool write_png(
	const char *filename,
	png_bytep *rows,
	int width,
	int height,
	int color_type,
	int bit_depth,
	png_colorp palette,
	int num_palette
	)
{
	PngContext ctx;
	memset(&ctx, 0, sizeof(ctx));

	int error_code = PNG_OK;

	ctx.fp = fopen(filename, "wb");
	if (!ctx.fp)
	{
		error_code = PNG_ERROR_CANNOT_OPEN_FILE;
		release_png_context(&ctx, error_code);
		return false;
	}

	ctx.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!ctx.png_ptr)
	{
		error_code = PNG_ERROR_CREATE_WRITE_STRUCT;
		release_png_context(&ctx, error_code);
		return false;
	}

	ctx.info_ptr = png_create_info_struct(ctx.png_ptr);
	if (!ctx.info_ptr)
	{
		error_code = PNG_ERROR_CREATE_INFO_STRUCT_WRITE;
		release_png_context(&ctx, error_code);
		return false;
	}

	if (setjmp(png_jmpbuf(ctx.png_ptr)))
	{
		error_code = PNG_ERROR_WRITE;
		release_png_context(&ctx, error_code);
		return false;
	}

	png_init_io(ctx.png_ptr, ctx.fp);
	png_set_IHDR(ctx.png_ptr,
	             ctx.info_ptr,
	             width,
	             height,
	             bit_depth,
	             color_type,
	             PNG_INTERLACE_NONE,
	             PNG_COMPRESSION_TYPE_DEFAULT,
	             PNG_FILTER_TYPE_DEFAULT);

	if (color_type == PNG_COLOR_TYPE_PALETTE && palette && num_palette > 0)
	{
		png_set_PLTE(ctx.png_ptr, ctx.info_ptr, palette, num_palette);
	}

	png_write_info(ctx.png_ptr, ctx.info_ptr);
	png_write_image(ctx.png_ptr, rows);
	png_write_end(ctx.png_ptr, ctx.info_ptr);

	png_destroy_write_struct(&ctx.png_ptr, &ctx.info_ptr);
	if (ctx.fp)
	{
		fclose(ctx.fp);
		ctx.fp = NULL;
	}

	return true;
}

void free_png_rows(png_bytep *rows, int height)
{
	if (!rows)
		return;

	for (int y = 0; y < height; y++)
	{
		free(rows[y]);
	}
	free(rows);
}
