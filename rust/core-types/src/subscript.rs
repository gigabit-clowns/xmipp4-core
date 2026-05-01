// SPDX-License-Identifier: GPL-3.0-only

//! Compatibility facade for subscript-related types and helpers.
//!
//! New code should import from `crate::dynamic_subscript` and `crate::slice`.

pub use crate::dynamic_subscript::{ellipsis, new_axis, DynamicSubscript};
pub use crate::slice::{
	all,
	end,
	even,
	make_slice,
	make_slice_full,
	make_slice_start,
	odd,
	sanitize_slice,
	Slice,
};
