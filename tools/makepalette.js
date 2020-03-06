const palette = []

const NUM_SHADES = 8;
for (let i = 0; i < NUM_SHADES; i++) {
  const s = parseInt(i / (NUM_SHADES - 1) * 255)
  palette.push([s, s, s])
}

const NUM_HUES = 30
const HUE_SHADES = [28, 113, 255, 384]
for (let i = 0; i < NUM_HUES; i++) {
  const hue = parseInt(i / NUM_HUES * 6)
  const f = i / NUM_HUES * 6 - hue
  const fUp = parseInt(Math.pow(f, 2) * 255);
  const fDown = parseInt(Math.pow(1 - f, 2) * 255)
  
  let r, g, b
  switch (hue) {
    case 0:
      r = 255
      g = fUp
      b = 0
      break
    case 1:
      r = fDown
      g = 255
      b = 0
      break
    case 2:
      r = 0
      g = 255
      b = fUp
      break
    case 3:
      r = 0
      g = fDown
      b = 255
      break
    case 4:
      r = fUp
      g = 0
      b = 255
      break
    case 5:
      r = 255
      g = 0
      b = fDown
      break
  }

  HUE_SHADES.forEach(shade => {
    const lo = Math.max(0, shade - 256)
    const hi = Math.min(255, shade)

    palette.push([r, g, b].map(c => parseInt(lo + c * (hi - lo) / 255)))
  })
}

palette.forEach(c => console.log(`CRGB(${c.join(', ')}),`))