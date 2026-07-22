# Selectable PsyCross SPU backends

This change preserves the original PsyCross OpenAL implementation as the
default **Legacy** backend and adds three optional software SPU renderers.

| Mode | Purpose | Native DSP rate | Recommended output |
| --- | --- | ---: | ---: |
| **Authentic** | Emulates the original PSX SPU algorithms, arithmetic, and quirks | 44.1 kHz | 44.1 kHz |
| **High Precision** | Preserves the original PSX DSP algorithms and character using modern numerical precision | 176.4 kHz | 176.4 kHz |
| **Modern** | Recreates the intended result using contemporary high-quality DSP designs | 352.8 kHz | 176.4 or 352.8 kHz |

The full per-feature implementation table and unsupported-hardware list are
included in `docs/software-spu.md` in the PsyCross change.

## Compatibility approach

- Legacy remains the default.
- The original `PsyX_SPUAL.cpp`, `PsyX_SPUAL.h`, Silent Hill configuration,
  launcher, console commands, and legacy XA player remain byte-identical.
- New behavior is isolated in new backend, DSP, configuration, and XA modules.
- Existing config files without `spu_renderer` continue to use Legacy.
- Old `exact`, `ideal`, and `reference` values remain accepted as aliases for
  `authentic`, `high_precision`, and `modern`.

## Frequency-response comparisons

These graphs use deterministic synthetic SPU/XA stress signals rendered by the
production DSP modules. They are relative spectra intended to highlight
differences between modes, not measurements of game assets or claims that one
response is universally preferable.

### Fractional pitch

![Fractional-pitch spectrum](comparison/01_fractional_pitch_spectrum.png)

### Maximum-pitch aliasing

![Maximum-pitch alias spectrum](comparison/02_max_pitch_alias_spectrum.png)

### Dense 24-voice mix

![Dense-mix spectrum](comparison/03_dense_mix_headroom_spectrum.png)

### Low-level Gaussian arithmetic

![Low-level Gaussian spectrum](comparison/04_low_level_gaussian_spectrum.png)

### Room reverb

![Room-reverb spectrum](comparison/05_room_reverb_spectrum.png)

### XA resampling

![XA-resampling spectrum](comparison/06_xa_resampling_spectrum.png)

Raw measurements are provided alongside the images:

- `comparison/measurements.csv`
- `comparison/pairwise_differences.csv`

## Scope and limitations

The software SPU implements 24 voices, SPU ADPCM, Gaussian reconstruction,
ADSR, signed volume and sweeps, pitch modulation, noise, key state, ENDX,
decoded CD/XA input, and the RAM-backed PSX reverb algorithm in Authentic.

It does not currently implement SPU IRQ behavior, DMA/FIFO cycle timing, or the
four hardware capture buffers. Authentic describes the implemented documented
digital path; it is not a claim of fully verified silicon equivalence.

## Branches under review

### PsyCross

- Branch: `copilot/software-spu-backends`
- Base: `019a1174e9f3db5980900a18335524bbbfe2703a`
- Head: `668f632d4465e8e91ed1e2ed736f29f76617bb35`
- Existing files touched: 2
- New files: 35

### Silent Hill integration

- Branch: `copilot/software-spu-integration`
- Base: `a5612545ed64e174415c383dec0e2df41d9e1fc9`
- Head: `cef7d699acbcb645a26b3b3c730223d79b4fa654`
- Existing files touched: 4
- New files: 5
